#include "catch.hpp"

#include <thrift/transport/TTransportException.h>
#include <thread>
#include "../src/rpc/directory_client.h"
#include "../src/rpc/directory_rpc_server.h"
#include "../src/tree/random_block_allocator.h"

using namespace ::elasticmem::directory;
using namespace ::apache::thrift::transport;

#define NUM_BLOCKS 1
#define HOST "127.0.0.1"
#define PORT 9090

static void wait_till_server_ready(const std::string &host, int port) {
  bool check = true;
  while (check) {
    try {
      directory_client(host, port);
      check = false;
    } catch (TTransportException &e) {
      usleep(100000);
    }
  }
}

TEST_CASE("rpc_create_directory_test", "[dir]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);
  REQUIRE_NOTHROW(tree.create_directories("/sandbox/1/2/a"));
  REQUIRE(tree.is_directory("/sandbox/1/2/a"));
  REQUIRE(tree.is_directory("/sandbox/1/2"));
  REQUIRE(tree.is_directory("/sandbox/1"));
  REQUIRE(tree.is_directory("/sandbox"));

  REQUIRE_NOTHROW(tree.create_directory("/sandbox/1/2/b"));
  REQUIRE(tree.is_directory("/sandbox/1/2/b"));

  REQUIRE_THROWS_AS(tree.create_directory("/sandbox/1/1/b"), directory_rpc_service_exception);

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_create_file_test", "[file][dir]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);
  REQUIRE_NOTHROW(tree.create_file("/sandbox/a.txt", "/tmp"));
  REQUIRE(tree.is_regular_file("/sandbox/a.txt"));

  REQUIRE_NOTHROW(tree.create_file("/sandbox/foo/bar/baz/a", "/tmp"));
  REQUIRE(tree.is_regular_file("/sandbox/foo/bar/baz/a"));

  REQUIRE_THROWS_AS(tree.create_file("/sandbox/foo/bar/baz/a/b", "/tmp"), directory_rpc_service_exception);
  REQUIRE_THROWS_AS(tree.create_directories("/sandbox/foo/bar/baz/a/b"),
                    directory_rpc_service_exception);

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_exists_test", "[file][dir]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);

  REQUIRE_NOTHROW(tree.create_file("/sandbox/file", "/tmp"));
  REQUIRE(tree.exists("/sandbox"));
  REQUIRE(tree.exists("/sandbox/file"));
  REQUIRE(!tree.exists("/sandbox/foo"));

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_file_size", "[file][dir][grow][shrink]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);

  REQUIRE_NOTHROW(tree.create_file("/sandbox/file", "/tmp"));
  REQUIRE(tree.file_size("/sandbox/file") == 0);
  REQUIRE_NOTHROW(t->grow("/sandbox/file", 20));
  REQUIRE(tree.file_size("/sandbox/file") == 20);
  REQUIRE_NOTHROW(t->shrink("/sandbox/file", 5));
  REQUIRE(tree.file_size("/sandbox/file") == 15);

  REQUIRE_NOTHROW(tree.create_file("/sandbox/file2", "/tmp"));
  REQUIRE_NOTHROW(t->grow("/sandbox/file2", 20));
  REQUIRE(tree.file_size("/sandbox") == 35);

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_last_write_time_test", "[file][dir][touch]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);

  std::uint64_t before = detail::now_ms();
  REQUIRE_NOTHROW(tree.create_file("/sandbox/file", "/tmp"));
  std::uint64_t after = detail::now_ms();
  REQUIRE(before <= tree.last_write_time("/sandbox/file"));
  REQUIRE(tree.last_write_time("/sandbox/file") <= after);

  before = detail::now_ms();
  REQUIRE_NOTHROW(t->touch("/sandbox/file"));
  after = detail::now_ms();
  REQUIRE(before <= tree.last_write_time("/sandbox/file"));
  REQUIRE(tree.last_write_time("/sandbox/file") <= after);

  before = detail::now_ms();
  REQUIRE_NOTHROW(t->touch("/sandbox"));
  after = detail::now_ms();
  REQUIRE(before <= tree.last_write_time("/sandbox"));
  REQUIRE(tree.last_write_time("/sandbox") <= after);
  REQUIRE(before <= tree.last_write_time("/sandbox/file"));
  REQUIRE(tree.last_write_time("/sandbox/file") <= after);
  REQUIRE(tree.last_write_time("/sandbox") == tree.last_write_time("/sandbox/file"));

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_permissions_test", "[file][dir]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);

  REQUIRE_NOTHROW(tree.create_file("/sandbox/file", "/tmp"));
  REQUIRE(tree.permissions("/sandbox") == perms::all);
  REQUIRE(tree.permissions("/sandbox/file") == perms::all);

  REQUIRE_NOTHROW(tree.permissions("/sandbox/file", perms::owner_all | perms::group_all, perm_options::replace));
  REQUIRE(tree.permissions("/sandbox/file") == (perms::owner_all | perms::group_all));

  REQUIRE_NOTHROW(tree.permissions("/sandbox/file", perms::others_all, perm_options::add));
  REQUIRE(tree.permissions("/sandbox/file") == (perms::owner_all | perms::group_all | perms::others_all));

  REQUIRE_NOTHROW(tree.permissions("/sandbox/file", perms::group_all | perms::others_all, perm_options::remove));
  REQUIRE(tree.permissions("/sandbox/file") == perms::owner_all);

  REQUIRE_NOTHROW(tree.permissions("/sandbox", perms::owner_all | perms::group_all, perm_options::replace));
  REQUIRE(tree.permissions("/sandbox") == (perms::owner_all | perms::group_all));

  REQUIRE_NOTHROW(tree.permissions("/sandbox", perms::others_all, perm_options::add));
  REQUIRE(tree.permissions("/sandbox") == (perms::owner_all | perms::group_all | perms::others_all));

  REQUIRE_NOTHROW(tree.permissions("/sandbox", perms::group_all | perms::others_all, perm_options::remove));
  REQUIRE(tree.permissions("/sandbox") == perms::owner_all);

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_remove_test", "[file][dir]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);

  REQUIRE_NOTHROW(tree.create_file("/sandbox/abcdef/example/a/b", "/tmp"));

  REQUIRE_NOTHROW(tree.remove("/sandbox/abcdef/example/a/b"));
  REQUIRE(!tree.exists("/sandbox/abcdef/example/a/b"));

  REQUIRE_NOTHROW(tree.remove("/sandbox/abcdef/example/a"));
  REQUIRE(!tree.exists("/sandbox/abcdef/example/a"));

  REQUIRE_THROWS_AS(tree.remove("/sandbox/abcdef"), directory_rpc_service_exception);
  REQUIRE(tree.exists("/sandbox/abcdef"));

  REQUIRE_NOTHROW(tree.remove_all("/sandbox/abcdef"));
  REQUIRE(!tree.exists("/sandbox/abcdef"));

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_rename_test", "[file][dir]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);
  REQUIRE_NOTHROW(tree.create_file("/sandbox/from/file1.txt", "/tmp"));
  REQUIRE_NOTHROW(tree.create_directory("/sandbox/to"));

  REQUIRE_THROWS_AS(tree.rename("/sandbox/from/file1.txt", "/sandbox/to/"), directory_rpc_service_exception);
  REQUIRE_NOTHROW(tree.rename("/sandbox/from/file1.txt", "/sandbox/to/file2.txt"));
  REQUIRE(tree.exists("/sandbox/to/file2.txt"));
  REQUIRE(!tree.exists("/sandbox/from/file1.txt"));

  REQUIRE_THROWS_AS(tree.rename("/sandbox/from", "/sandbox/to"), directory_rpc_service_exception);
  REQUIRE_NOTHROW(tree.rename("/sandbox/from", "/sandbox/to/subdir"));
  REQUIRE(tree.exists("/sandbox/to/subdir"));
  REQUIRE(!tree.exists("/sandbox/from"));

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_status_test", "[file][dir]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);
  std::uint64_t before = detail::now_ms();
  REQUIRE_NOTHROW(tree.create_file("/sandbox/file", "/tmp"));
  std::uint64_t after = detail::now_ms();
  REQUIRE(tree.status("/sandbox/file").permissions() == perms::all);
  REQUIRE(tree.status("/sandbox/file").type() == file_type::regular);
  REQUIRE(before <= tree.status("/sandbox/file").last_write_time());
  REQUIRE(tree.status("/sandbox/file").last_write_time() <= after);

  before = detail::now_ms();
  REQUIRE_NOTHROW(tree.create_directory("/sandbox/dir"));
  after = detail::now_ms();
  REQUIRE(tree.status("/sandbox/dir").permissions() == perms::all);
  REQUIRE(tree.status("/sandbox/dir").type() == file_type::directory);
  REQUIRE(before <= tree.status("/sandbox/dir").last_write_time());
  REQUIRE(tree.status("/sandbox/dir").last_write_time() <= after);

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_directory_entries_test", "[file][dir]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);

  std::uint64_t t0 = detail::now_ms();
  REQUIRE_NOTHROW(tree.create_directories("/sandbox/a/b"));
  std::uint64_t t1 = detail::now_ms();
  REQUIRE_NOTHROW(tree.create_file("/sandbox/file1.txt", "/tmp"));
  std::uint64_t t2 = detail::now_ms();
  REQUIRE_NOTHROW(tree.create_file("/sandbox/file2.txt", "/tmp"));
  std::uint64_t t3 = detail::now_ms();

  std::vector<directory_entry> entries;
  REQUIRE_NOTHROW(entries = tree.directory_entries("/sandbox"));
  REQUIRE(entries.size() == 3);
  REQUIRE(entries.at(0).name() == "a");
  REQUIRE(entries.at(0).type() == file_type::directory);
  REQUIRE(entries.at(0).permissions() == perms::all);
  REQUIRE(t0 <= entries.at(0).last_write_time());
  REQUIRE(entries.at(0).last_write_time() <= t1);
  REQUIRE(entries.at(1).name() == "file1.txt");
  REQUIRE(entries.at(1).type() == file_type::regular);
  REQUIRE(entries.at(1).permissions() == perms::all);
  REQUIRE(entries.at(1).last_write_time() <= t2);
  REQUIRE(t1 <= entries.at(1).last_write_time());
  REQUIRE(entries.at(2).name() == "file2.txt");
  REQUIRE(entries.at(2).type() == file_type::regular);
  REQUIRE(entries.at(2).permissions() == perms::all);
  REQUIRE(t2 <= entries.at(2).last_write_time());
  REQUIRE(entries.at(2).last_write_time() <= t3);

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_recursive_directory_entries_test", "[file][dir]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);

  std::uint64_t t0 = detail::now_ms();
  REQUIRE_NOTHROW(tree.create_directories("/sandbox/a/b"));
  std::uint64_t t1 = detail::now_ms();
  REQUIRE_NOTHROW(tree.create_file("/sandbox/file1.txt", "/tmp"));
  std::uint64_t t2 = detail::now_ms();
  REQUIRE_NOTHROW(tree.create_file("/sandbox/file2.txt", "/tmp"));
  std::uint64_t t3 = detail::now_ms();

  std::vector<directory_entry> entries;
  REQUIRE_NOTHROW(entries = tree.recursive_directory_entries("/sandbox"));
  REQUIRE(entries.size() == 4);
  REQUIRE(entries.at(0).name() == "a");
  REQUIRE(entries.at(0).type() == file_type::directory);
  REQUIRE(entries.at(0).permissions() == perms::all);
  REQUIRE(t0 <= entries.at(0).last_write_time());
  REQUIRE(entries.at(0).last_write_time() <= t1);
  REQUIRE(entries.at(1).name() == "b");
  REQUIRE(entries.at(1).type() == file_type::directory);
  REQUIRE(entries.at(1).permissions() == perms::all);
  REQUIRE(t0 <= entries.at(1).last_write_time());
  REQUIRE(entries.at(1).last_write_time() <= t1);
  REQUIRE(entries.at(2).name() == "file1.txt");
  REQUIRE(entries.at(2).type() == file_type::regular);
  REQUIRE(entries.at(2).permissions() == perms::all);
  REQUIRE(t1 <= entries.at(2).last_write_time());
  REQUIRE(entries.at(2).last_write_time() <= t2);
  REQUIRE(entries.at(3).name() == "file2.txt");
  REQUIRE(entries.at(3).type() == file_type::regular);
  REQUIRE(entries.at(3).permissions() == perms::all);
  REQUIRE(t2 <= entries.at(3).last_write_time());
  REQUIRE(entries.at(3).last_write_time() <= t3);

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_dstatus_test", "[file]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);
  REQUIRE_NOTHROW(tree.create_file("/sandbox/file.txt", "/tmp"));
  REQUIRE_THROWS_AS(tree.dstatus("/sandbox"), directory_rpc_service_exception);
  REQUIRE(tree.dstatus("/sandbox/file.txt").mode() == storage_mode::in_memory);
  REQUIRE(tree.dstatus("/sandbox/file.txt").persistent_store_prefix() == "/tmp");
  REQUIRE(tree.dstatus("/sandbox/file.txt").data_blocks().size() == 1);

  data_status status(storage_mode::in_memory_grace, "/tmp2", {"a", "b", "c", "d"});
  REQUIRE_NOTHROW(t->dstatus("/sandbox/file.txt", status));
  REQUIRE(tree.dstatus("/sandbox/file.txt").mode() == storage_mode::in_memory_grace);
  REQUIRE(tree.dstatus("/sandbox/file.txt").persistent_store_prefix() == "/tmp2");
  REQUIRE(tree.dstatus("/sandbox/file.txt").data_blocks().size() == 4);
  REQUIRE(tree.dstatus("/sandbox/file.txt").data_blocks().at(0) == "a");
  REQUIRE(tree.dstatus("/sandbox/file.txt").data_blocks().at(1) == "b");
  REQUIRE(tree.dstatus("/sandbox/file.txt").data_blocks().at(2) == "c");
  REQUIRE(tree.dstatus("/sandbox/file.txt").data_blocks().at(3) == "d");

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_storage_mode_test", "[file]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);

  REQUIRE_NOTHROW(tree.create_file("/sandbox/file.txt", "/tmp"));
  REQUIRE_THROWS_AS(tree.mode("/sandbox"), directory_rpc_service_exception);
  REQUIRE(tree.mode("/sandbox/file.txt") == storage_mode::in_memory);

  REQUIRE_NOTHROW(t->mode("/sandbox/file.txt", storage_mode::in_memory_grace));
  REQUIRE(tree.mode("/sandbox/file.txt") == storage_mode::in_memory_grace);

  REQUIRE_NOTHROW(t->mode("/sandbox/file.txt", storage_mode::flushing));
  REQUIRE(tree.mode("/sandbox/file.txt") == storage_mode::flushing);

  REQUIRE_NOTHROW(t->mode("/sandbox/file.txt", storage_mode::on_disk));
  REQUIRE(tree.mode("/sandbox/file.txt") == storage_mode::on_disk);

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_blocks_test", "[file]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);

  REQUIRE_NOTHROW(tree.create_file("/sandbox/file.txt", "/tmp"));
  REQUIRE_THROWS_AS(tree.data_blocks("/sandbox"), directory_rpc_service_exception);
  REQUIRE(tree.data_blocks("/sandbox/file.txt").size() == 1);

  REQUIRE_NOTHROW(t->add_data_block("/sandbox/file.txt"));
  REQUIRE_NOTHROW(t->add_data_block("/sandbox/file.txt"));
  REQUIRE_NOTHROW(t->add_data_block("/sandbox/file.txt"));
  REQUIRE_THROWS_AS(t->add_data_block("/sandbox/file.txt"), std::out_of_range);
  std::vector<std::string> file_blocks;
  REQUIRE_NOTHROW(file_blocks = tree.data_blocks("/sandbox/file.txt"));
  std::sort(file_blocks.begin(), file_blocks.end());
  REQUIRE(file_blocks.size() == 4);
  REQUIRE(file_blocks.at(0) == "a");
  REQUIRE(file_blocks.at(1) == "b");
  REQUIRE(file_blocks.at(2) == "c");
  REQUIRE(file_blocks.at(3) == "d");
  REQUIRE(alloc->num_free_blocks() == 0);
  REQUIRE(alloc->num_allocated_blocks() == 4);

  REQUIRE_NOTHROW(t->remove_data_block("/sandbox/file.txt", "c"));
  REQUIRE_NOTHROW(file_blocks = tree.data_blocks("/sandbox/file.txt"));
  std::sort(file_blocks.begin(), file_blocks.end());
  REQUIRE(file_blocks.size() == 3);
  REQUIRE(file_blocks.at(0) == "a");
  REQUIRE(file_blocks.at(1) == "b");
  REQUIRE(file_blocks.at(2) == "d");
  REQUIRE(alloc->num_free_blocks() == 1);
  REQUIRE(alloc->num_allocated_blocks() == 3);

  REQUIRE_NOTHROW(t->remove_all_data_blocks("/sandbox/file.txt"));
  REQUIRE(tree.data_blocks("/sandbox/file.txt").empty());
  REQUIRE(alloc->num_free_blocks() == 4);
  REQUIRE(alloc->num_allocated_blocks() == 0);

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}

TEST_CASE("rpc_file_type_test", "[file][dir]") {
  std::vector<std::string> blocks = {"a", "b", "c", "d"};
  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto server = directory_rpc_server::create(t, HOST, PORT);
  std::thread serve_thread([&server] { server->serve(); });
  wait_till_server_ready(HOST, PORT);

  directory_client tree(HOST, PORT);

  REQUIRE_NOTHROW(tree.create_file("/sandbox/file.txt", "/tmp"));
  REQUIRE(tree.is_regular_file("/sandbox/file.txt"));
  REQUIRE_FALSE(tree.is_directory("/sandbox/file.txt"));

  REQUIRE(tree.is_directory("/sandbox"));
  REQUIRE_FALSE(tree.is_regular_file("/sandbox"));

  server->stop();
  if (serve_thread.joinable()) {
    serve_thread.join();
  }
}