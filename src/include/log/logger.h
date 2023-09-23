#ifndef NEXT_LOGGER_H
#define NEXT_LOGGER_H

#include <chrono>
#include <string>
#include <functional>
#include <deque>
#include <atomic>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <iomanip>
#include <algorithm>
#include <iostream>
#include "../core/utils.h"

namespace Next{

enum class LogLevel { INFO, WARNING, ERROR, FATAL };

constexpr int COUNT_THRESHOLD = 1000;

constexpr std::chrono::duration REFRESH_THRESHOLD = std::chrono::milliseconds(3000); 

const std::string  LOG_PATH = std::string("NextLog");

class Logger{
public:
    static auto GetInstance() noexcept -> Logger &;

    static void LogMsg(LogLevel loglevel, const std::string &msg) noexcept;

    NON_COPYABLE(Logger);

    struct Log{
        std::string stamped_msg_;

        Log(LogLevel loglevel, const std::string &msg) noexcept;

        friend auto operator<<(std::ostream &os,const Log &log) -> std::ostream &{
            os << log.stamped_msg_;
            return os;
        }
    };
private:
    explicit Logger(const std::function<void(const std::deque<Log> &logs)> &log_strategy);
    ~Logger();
    void PushLog(Log &&log);
    void LogWriting();

    std::function<void(const std::deque<Log> &logs)> log_strategy_;
    std::atomic<bool> done_ = false;
    std::mutex mtx_;
    std::condition_variable cv_;
    std::deque<Log> queue_;
    std::thread log_writer_;
    std::chrono::microseconds last_flush_;
};

#ifdef NOLOG
#define LOG_INFO(x) {};
#define LOG_WARNING(x) {};
#define LOG_ERROR(x) {};
#define LOG_FATAL(x) {};
#else
#define LOG_INFO(x) Next::Logger::LogMsg(Next::LogLevel::INFO, x);
#define LOG_WARNING(x) Next::Logger::LogMsg(Next::LogLevel::WARNING, x);
#define LOG_ERROR(x) Next::Logger::LogMsg(Next::LogLevel::ERROR, x);
#define LOG_FATAL(x) Next::Logger::LogMsg(Next::LogLevel::FATAL, x);
#endif

} // namespace Netx


#endif // !NEXT_LOGGER_H