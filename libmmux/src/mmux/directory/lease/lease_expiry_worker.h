#ifndef MMUX_LEASE_MANAGER_H
#define MMUX_LEASE_MANAGER_H

#include <thread>
#include "../fs/directory_tree.h"

namespace mmux {
namespace directory {
/* Lease_expiry worker class */
class lease_expiry_worker {
 public:
  lease_expiry_worker(std::shared_ptr<directory_tree> tree, uint64_t lease_period_ms, uint64_t grace_period_ms);
  ~lease_expiry_worker();

  void start();
  void stop();
  size_t num_epochs() const;
 private:
  void remove_expired_leases();

  void remove_expired_nodes(std::shared_ptr<ds_dir_node> parent,
                            const std::string &parent_path,
                            const std::string &child_name,
                            std::uint64_t epoch);
  /* Lease duration */
  std::chrono::milliseconds lease_period_ms_;
  /* Extended lease duration */
  std::chrono::milliseconds grace_period_ms_;
  /* Directory tree */
  std::shared_ptr<directory_tree> tree_;
  /* Worker thread */
  std::thread worker_;
  /* Stop bool */
  std::atomic_bool stop_;
  /* number epochs */
  std::atomic_size_t num_epochs_;
};

}
}

#endif //MMUX_LEASE_MANAGER_H
