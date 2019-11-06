/** @file EventStaticDemo.h
 *  @brief Demo event static functionality
 *  
 *  Static event could be assigned to any global or function and invoked in the same thread where it was invoked.
 *  It could be used to break synchronius sequence with async call.
 *  In this demo thread created and invoked call ns_EventDemo::startEventDemo() from main thread to initiate demo. 
 *  You can't create event in main threadm because it not registered in threads dispathcher.
 *  After demo started thread invoke 4 different events. Events called asyncroniusly in the same thread.
 * 
 *  Also there is example how to call lambda function asynchronously in the same thread. For this no any declarations needed.
 * 
 *  @date 2019
 *  @author Alexander Geronime 
 */

#include <iostream>
#include <string>
#include <chrono>
#include <memory>

#include "../itc.hpp"

namespace ns_EventStaticDemo {

    static const std::string THREAD = "EventStaticDemo_thread";

    struct A
    {
        A(int i, std::string s): mI(i), mS(s) {}
        int mI;
        std::string mS;
    };

    std::ostream& operator<<(std::ostream &o, std::shared_ptr<A> a)
    {
        return o << "{i=" << a->mI << ", s=" << a->mS << "}";
    }

    void func1() 
    {
        // wait a little to syncronize outputs
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        std::cout << itc::currentThreadName() << ": EventConsumer::func1()" << std::endl;
    }

    void func2(int i)
    { 
        std::cout << itc::currentThreadName() << ": EventConsumer::func2(" << i << ")" << std::endl; 
    }

    void func3(std::string s, int i, float f) 
    { 
        std::cout << itc::currentThreadName() << ": EventConsumer::func3(" << s << ", " << i << ", " << f << ")" << std::endl;
    }

    void func4(std::shared_ptr<A> a) 
    {
        std::cout << itc::currentThreadName() << ": EventConsumer::func4(" << a << ")" << std::endl;
    }

}

DECLARE_STATIC_EVENT (STATIC_EVENT_func1, ns_EventStaticDemo::func1)
DECLARE_STATIC_EVENT (STATIC_EVENT_func2, ns_EventStaticDemo::func2, int)
DECLARE_STATIC_EVENT (STATIC_EVENT_func3, ns_EventStaticDemo::func3, std::string, int, float)
DECLARE_STATIC_EVENT (STATIC_EVENT_func4, ns_EventStaticDemo::func4, std::shared_ptr<ns_EventStaticDemo::A>)


namespace ns_EventStaticDemo {
    void startEventDemo() {
        itc::invoke(STATIC_EVENT_func1::Event());
        itc::invoke(STATIC_EVENT_func2::Event(5));
        itc::invoke(STATIC_EVENT_func3::Event("HELLO", 42, 5.5f));
        itc::invoke(STATIC_EVENT_func4::Event(std::make_shared<ns_EventStaticDemo::A>(33, "Hello A")));

        // Example how to invoke lambda async in current thread
        itc::invoke(itc::InlineEvent<int, std::string>([](int i, std::string s){ std::cout << "L: i=" << i << ", s=" << s << std::endl;}, 7, "I'm in lambda"));
    }
}

DECLARE_STATIC_CALL (STATIC_CALL_SED_func1, ns_EventStaticDemo::THREAD, ns_EventStaticDemo::startEventDemo)

class EventStaticDemo
{
public:

    void run();
};

void EventStaticDemo::run()
{
    itc::createEventLoop(ns_EventStaticDemo::THREAD);

    itc::invoke(STATIC_CALL_SED_func1::CallStatic());
}