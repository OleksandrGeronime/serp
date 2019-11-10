#include <thread>
#include <chrono>

#include "TestDemo.hpp"

int main()
{
    TestDemo testDemo;
    testDemo.run();

    while(true) {              
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
    
    return 0;
}
