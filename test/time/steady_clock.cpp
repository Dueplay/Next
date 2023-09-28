#include <chrono>
#include <iostream>
int main()
{
    // 获取开始时间点
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    // 需要获取程序耗时的代码段
    std::cout << "print 1000 stars ...." << std::endl;
    for (int i = 0; i < 1000; ++i)
    {
        std::cout << "*";
    }
    std::cout << std::endl;
    // 获取结束时间点
    std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();
    // 计算差值，是一个时间间隔
    auto dt = last - start;
    // dt.count()返回时间间隔中有多少个时钟周期
    std::cout << "总共耗时: " << dt.count() << "纳秒" << std::endl;
    std::cout << "总共耗时: " << dt.count() / (double)1000000 << "ms" << std::endl;
}
