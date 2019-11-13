#include <thread>
#include <chrono>

#include "CallStaticDemo.hpp"
#include "CallDemo.hpp"
#include "EventStaticDemo.hpp"
#include "EventDemo.hpp"
#include "RequestDemo.hpp"
#include "TimerDemo.hpp"

int main()
{
    CallStaticDemo callStaticDemo;
    CallDemo callDemo;
    EventStaticDemo eventStaticDemo;
    EventDemo eventDemo;
    RequestDemo requestDemo;
    TimerDemo timerDemo;
    
    // Uncoment demos by one for better log output, otherwise log outputs from different threads could be mixed
    // callStaticDemo.run();
    // callDemo.run();
    // eventStaticDemo.run();
    // eventDemo.run();
    // requestDemo.run();
     timerDemo.run();


    while(true) {              
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
    
    return 0;
}
