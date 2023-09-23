#include "../../src/include/log/logger.h"
using Next::Logger;
// 编译：
// g++ test/log/log_test.cpp src/log/logger.cpp -o log_test -pthread -std=c++17 -Isrc/include/log
int main(){
    LOG_INFO("This is an informational message.");
    LOG_WARNING("This is a warning message.");
    LOG_ERROR("This is an error message.");
    LOG_FATAL("This is a fatal error message.");
    return 0;
}

