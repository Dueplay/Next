#include "core/thread_pool.h"

namespace Next {
thread_pool::thread_pool(int size = std::thread::hardware_concurrency() -1) {
    size = std::max(size, MIN_NUM_THREADS_IN_POOL);
    for (int i = 0; i < size; i++) {
        threads_.emplace_back([this]() {
            while(true) {
                std::function<void()> next_task;
                {
                    std::unique_lock<std::mutex> lock(mtx_);
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

thread_pool::~thread_pool() {
    Exit();
    for (auto &worker : threads_) {
        if (worker.joinable()) {
            worker.join();
        }
    }

}

void thread_pool::Exit() {
    exit_ = true;
    cv_.notify_all();//唤醒阻塞在cv.wait的所有线程
}

auto thread_pool::GetSize() const noexcept -> size_t { return threads_.size(); }
}