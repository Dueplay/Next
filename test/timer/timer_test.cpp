#include "../../src/include/core/timer.h"
#include <iostream>
int main() {
    Next::Timer timer;
    std::function<void()> f = []() {
        std::cout << "this a singtimer" << endl;
    };

    timer.AddSingleTimer(3, f);
}