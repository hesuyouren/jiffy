#include <iostream>
#include <vector>
#include <thread>
#include <directory/tree/directory_tree.h>
#include <directory/tree/random_block_allocator.h>
#include <directory/rpc/directory_rpc_server.h>
#include <directory/lease/lease_manager.h>
#include <directory/rpc/directory_lease_server.h>
#include <kv/manager/kv_manager.h>

using namespace ::elasticmem::directory;
using namespace ::elasticmem::kv;

int main() {
  std::string host = "0.0.0.0";
  int service_port = 9090;
  int lease_port = 9091;
  uint64_t lease_period_ms = 10000;
  uint64_t grace_period_ms = 10000;
  std::vector<std::string> blocks;

  // TODO: Fix
  for (std::size_t i = 0; i < 8; i++) {
    blocks.push_back("127.0.0.0:9092:" + std::to_string(i));
  }

  auto alloc = std::make_shared<random_block_allocator>(blocks);
  auto t = std::make_shared<directory_tree>(alloc);
  auto directory_server = directory_rpc_server::create(t, host, service_port);
  std::thread directory_serve_thread([&directory_server] {
    try {
      directory_server->serve();
    } catch (std::exception &e) {
      std::cerr << "Directory server error: " << e.what() << std::endl;
    }
  });

  std::cout << "Directory server listening on " << host << ":" << service_port << std::endl;

  auto kv = std::make_shared<kv_manager>();
  auto lease_server = directory_lease_server::create(t, kv, host, lease_port);
  std::thread lease_serve_thread([&lease_server] {
    try {
      lease_server->serve();
    } catch (std::exception &e) {
      std::cerr << "Lease server error: " << e.what() << std::endl;
    }
  });

  std::cout << "Lease server listening on " << host << ":" << lease_port << std::endl;

  lease_manager lmgr(t, lease_period_ms, grace_period_ms);
  lmgr.start();

  if (directory_serve_thread.joinable()) {
    directory_serve_thread.join();
  }

  if (lease_serve_thread.joinable()) {
    lease_serve_thread.join();
  }
  return 0;
}