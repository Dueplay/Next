#include "../include/log/logger.h"
#include <filesystem>
#include <fstream>

namespace Next{

const char *log_level_name[] = { "INFO: ", "WARNING: ", "ERROR: ", "FATAL: " };

auto GetCurrentTime() -> std::chrono::milliseconds {
    using namespace std::chrono;
    return duration_cast<milliseconds> (system_clock::now().time_since_epoch());
}

auto GetCurrentDate() -> std::string {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream stream;
    stream << std::put_time(&tm, "%d%b%Y");
    return stream.str();
}

void PrintToScreen(const std::deque<Logger::Log> &logs) {
    std::for_each(logs.begin(), logs.end(), [](const auto &log){
        std::cout << log;
    });
}

struct StreamWriter {
    std::fstream f_;

    explicit StreamWriter(const std::string &path = LOG_PATH) {
        f_.open(path + "_" + GetCurrentDate(), std::fstream::out | std::fstream::trunc);
    }

    ~StreamWriter() {
        if (f_.is_open()) {
            f_.flush();
            f_.close();
        }
    }

    void WriteLogs(const std::deque<Logger::Log> &logs) {
        std::for_each(logs.begin(), logs.end(), [this](auto &log){
            f_ << log;
        });
        f_.flush();
    }
};

void PrintToFile(const std::deque<Logger::Log> &logs) {
    static StreamWriter stream_writer;
    stream_writer.WriteLogs(logs);
}

// 构造一条log
Logger::Log::Log(LogLevel loglevel, const std::string &msg) noexcept{
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::stringstream stream;
    stream << std::put_time(&tm, "[%d %b %Y %H:%M:%S]") << log_level_name[static_cast<int>(loglevel)] << msg << std::endl;
    stamped_msg_ = stream.str();
}

void Logger::LogMsg(LogLevel loglevel, const std::string &msg) noexcept {
    auto log = Logger::Log(loglevel,msg);
    GetInstance().PushLog(std::move(log));
}

// 单列模式
auto Logger::GetInstance() noexcept -> Logger & {
    static Logger single_logger{PrintToFile};
    return single_logger;
}

Logger::Logger(const std::function<void(const std::deque<Log> &logs)> &log_strategy) {
    log_strategy_ = log_strategy;
    last_flush_ = GetCurrentTime();
    log_writer_ = std::thread(&Logger::LogWriting,this); // log_writer_线程函数为Logger::LogWriting
}

Logger::~Logger() {
    done_ = true;
    cv_.notify_one();
    if (log_writer_.joinable()) { //日志write线程和Logger线程还有关系，则log线程负责回收log_writer_
        log_writer_.join();
    }
}

void Logger::PushLog(Log &&log) {
    using std::chrono::milliseconds;
    bool should_notify = false;
    // {}为unique_lock的加锁范围，出这个{}代码块则解锁
    {
        std::unique_lock<std::mutex> lock(mtx_);
        queue_.push_back(std::move(log));
        milliseconds now = GetCurrentTime();
        if ((now - last_flush_) > REFRESH_THRESHOLD || queue_.size() > COUNT_THRESHOLD) {
            should_notify = true; // 通知消费者
        }
    }
    if (should_notify) {
        cv_.notify_one();
    }
}
void Logger::LogWriting() {
    std::deque<Logger::Log> write_queue;
    while(true) {
        std::unique_lock<std::mutex> lock(mtx_);
        cv_.wait(lock, [this]() {
            return done_ || queue_.size() > COUNT_THRESHOLD || GetCurrentTime() - last_flush_ > REFRESH_THRESHOLD;
        });

        if (!queue_.empty()) {
            write_queue.swap(queue_);
            lock.unlock();   // 生产者可以继续写log到队列中
            log_strategy_(write_queue);
            last_flush_ = GetCurrentTime();
            write_queue.clear();
        }
        if (done_) {
            return;
        }
    } 
}

} // namespace Next