/** @file EventDemo.h
 *  @brief Demo event functionality
 *  
 *  Event could be assigned to any public memember function and invoked in the same thread where it was ivoked.
 *  It could be used to break synchronius sequence with async call.
 *  In this demo thread created and invoked call ns_EventDemo::startEventDemo() from main thread to initiate demo. 
 *  You can't create event in main threadm because it not registered in threads dispathcher.
 *  After demo started thread invoke 4 different events. Events called asyncroniusly in the same thread.
 * 
 *  @date 2019
 *  @author Alexander Geronime 
 */

#include <iostream>
#include <string>
#include <chrono>
#include <memory>

#include "../itc.h"

namespace ns_EventDemo {

    static const std::string THREAD = "EventDemo_thread";

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

    struct EventConsumer 
    {
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
    };
}

DECLARE_EVENT (EVENT_func1, ns_EventDemo::EventConsumer, func1)
DECLARE_EVENT (EVENT_func2, ns_EventDemo::EventConsumer, func2, int)
DECLARE_EVENT (EVENT_func3, ns_EventDemo::EventConsumer, func3, std::string, int, float)
DECLARE_EVENT (EVENT_func4, ns_EventDemo::EventConsumer, func4, std::shared_ptr<ns_EventDemo::A>)

namespace ns_EventDemo {
    void startEventDemo() {
        ns_EventDemo::EventConsumer consumer;

        itc::invoke(EVENT_func1::Event(&consumer));
        itc::invoke(EVENT_func2::Event(&consumer, EVENT_func2::Params(5)));
        itc::invoke(EVENT_func3::Event(&consumer, EVENT_func3::Params("HELLO", 42, 5.5f)));
        itc::invoke(EVENT_func4::Event(&consumer, EVENT_func4::Params(std::make_shared<ns_EventDemo::A>(33, "Hello A"))));
    }
}

DECLARE_STATIC_CALL (STATIC_CALL_ED_func1, ns_EventDemo::THREAD, ns_EventDemo::startEventDemo)

class EventDemo
{
public:

    void run();
};

void EventDemo::run()
{
    itc::createEventLoop(ns_EventDemo::THREAD);

    itc::invoke(STATIC_CALL_ED_func1::CallStatic());
}