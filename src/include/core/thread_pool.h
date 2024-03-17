#ifndef NEXT_ThreadPool_H
#define NEXT_ThreadPool_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
#include "core/utils.h"
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
  using return_type =
      std::invoke_result_t<F, Args...>;  // invoke_result_t 用于在编译时推断函数调用的返回类型。确认F(args)的返回类型
  if (exit_) {
    throw std::runtime_error("ThreadPool: SubmitTask() called while already exit_ being true");
  }

  // packaged_task将函数（或任何可调用对象）封装为一个任务，可以在将来的某个时间点执行
  // 它非常适用于异步编程，因为它可以绑定一个返回值，当任务执行完毕时，这个返回值可以通过 std::future 对象来获取。
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
}  // namespace Next
#endif