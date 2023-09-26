#ifndef NEXT_ThreadPool_H
#define NEXT_ThreadPool_H

#include "core/utils.h"
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
namespace Next {
static constexpr int MIN_NUM_THREADS_IN_POOL = 2;

class ThreadPool {
public:
  explicit ThreadPool(int size = std::thread::hardware_concurrency() - 1);

  ~ThreadPool();

  NON_COPYABLE(ThreadPool);

  template <typename F, typename... Args>
  decltype(auto) SubmitTask(F &&new_task, Args &&...args);

  void Exit();

  auto GetSize() const noexcept -> size_t;

private:
  std::vector<std::thread> threads_;
  std::queue<std::function<void()>> tasks_;
  std::mutex mtx_;
  std::condition_variable cv_;
  std::atomic<bool> exit_{false};
};

template <typename F, typename... Args>
auto ThreadPool::SubmitTask(F &&new_task, Args &&...args) -> decltype(auto) {
  using return_type = std::invoke_result_t<F, Args...>;
  if (exit_) {
    throw std::runtime_error(
        "ThreadPool: SubmitTask() called while already exit_ being true");
  }

  auto packaged_new_task = std::make_shared<std::packaged_task<return_type()>>(
      std::bind(std::forward<F>(new_task), std::forward<Args>(args)...));
  auto fut = packaged_new_task->get_future();

  {
    std::unique_lock<std::mutex> lock(mtx_);
    tasks_.emplace([packaged_new_task]() { (*packaged_new_task)(); });
  }
  cv_.notify_one();
  return fut;
}
} // namespace Next
#endif