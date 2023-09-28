#include <chrono>
#include <iostream>
void f(){
    std::cout << "print 1000 stars ...." << std::endl;
    for (int i = 0; i < 1000; ++i)
    {
        std::cout << "*";
    }
    std::cout << std::endl;
}
int main()
{
    // 获取开始时间点
    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
    // 需要获取程序耗时的代码段
   	f();
    // 获取结束时间点
    std::chrono::steady_clock::time_point last = std::chrono::steady_clock::now();
    
    // 计算差值，是一个为多少ns的时间间隔
    auto dt = last - start;
    // dt.count()返回时间间隔中有多少个时钟周期
    std::cout << "总共耗时: " << dt.count() << "纳秒" << std::endl;
    
    // 转换
    // 整数时长：时钟周期纳秒转毫秒，小转大，并且是int转int，所以不能直接转，要求 duration_cast
    auto int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(dt);
     // 小数时长：int转double，符合第二点，虽然是小转大但不要求 duration_cast
    std::chrono::duration<double, std::ratio<1, 1000>> fp_ms = dt;
    
    std::cout << "f() took " << fp_ms.count() << " ms, "
        << "or " << int_ms.count() << " whole milliseconds\n"<< std::endl;

}