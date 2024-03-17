#include "core/thread_pool.h"

namespace Next {
ThreadPool::ThreadPool(int size) {
  size = std::max(size, MIN_NUM_THREADS_IN_POOL);
  for (int i = 0; i < size; i++) {
    threads_.emplace_back([this]() {
      while (true) {
        std::function<void()> next_task;
        {
          std::unique_lock<std::mutex> lock(mtx_);
          // wait 函数首先会评估谓词函数，如果谓词返回 true，则表示条件已满足，wait 函数将不会阻塞当前线程，直接返回。
          /**
           * 如果谓词返回 false，则 wait 函数会自动释放锁，并阻塞当前线程，直到其他线程在相同的 std::condition_variable
           * 对象上调用 notify_one 或 notify_all。一旦当前线程被唤醒（无论是由于通知还是因为spurious wakeups），wait
           * 函数会再次自动获取锁，并重新评估谓词。只有当谓词评估为 true 时，wait
           * 才会返回，这确保了在返回前条件已经满足。
           */
          cv_.wait(lock, [this]() { return exit_ || !tasks_.empty(); });
          if (exit_ && tasks_.empty()) {
            return;
          }

          next_task = tasks_.front();
          tasks_.pop();
        }
        next_task();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  Exit();
  for (auto &worker : threads_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

void ThreadPool::Exit() {
  exit_ = true;
  cv_.notify_all();  // 唤醒阻塞在cv.wait的所有线程
}

auto ThreadPool::GetSize() const noexcept -> size_t { return threads_.size(); }
}  // namespace Next