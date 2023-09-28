#include <chrono>
#include <iostream>
int main()
{
    // 新纪元1970.1.1时间
    std::chrono::system_clock::time_point epoch;

    
    std::chrono::duration<int,std::ratio<60 * 60 * 24>> day(1);
    // 新纪元1970.1.1时间 + 1天
    std::chrono::system_clock::time_point ppt(day);

    using day_t = std::chrono::duration<int, std::ratio<60 * 60 * 24>>;
    // 新纪元1970.1.1时间 + 10天
    // std::chrono::time_point需要指定模板参数
    std::chrono::time_point<std::chrono::system_clock,day_t> t(day_t(10));

    // 系统当前时间
    // std::chrono::system_clock::time_point = chrono::time_point<system_clock>
    std::chrono::system_clock::time_point today = std::chrono::system_clock::now();
    
    // 转换为time_t时间类型
    time_t tm = std::chrono::system_clock::to_time_t(today);
    std::cout << "今天的日期是:    " << ctime(&tm);

    time_t tm1 = std::chrono::system_clock::to_time_t(today+day);
    std::cout << "明天的日期是:    " << ctime(&tm1);

    time_t tm2 = std::chrono::system_clock::to_time_t(epoch);
    std::cout << "新纪元时间:      " << ctime(&tm2);

    time_t tm3 = std::chrono::system_clock::to_time_t(ppt);
    std::cout << "新纪元时间+1天:  " << ctime(&tm3);

    time_t tm4 = std::chrono::system_clock::to_time_t(t);
    std::cout << "新纪元时间+10天: " << ctime(&tm4);
}
