#include <thread>
#include <chrono>

#include "demo/CallStaticDemo.h"
#include "demo/CallDemo.h"
#include "demo/EventDemo.h"
#include "demo/RequestDemo.h"
#include "demo/TimerDemo.h"

int main()
{
    CallStaticDemo callStaticDemo;
    CallDemo callDemo;
    EventDemo eventDemo;
    RequestDemo requestDemo;
    TimerDemo timerDemo;
    
    // Uncoment demos by one for better log output, otherwise log outputs from different threads could be mixed
    callStaticDemo.run();   
    //callDemo.run();
    //eventDemo.run();
    //requestDemo.run();
    //timerDemo.run();

    while(true) {              
        std::this_thread::sleep_for(std::chrono::hours(1));
    }
    
    return 0;
}
