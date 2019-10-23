/** @file CallStaticDemo.h
 *  @brief Demo static call functionality
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#include <iostream>
#include <string>
#include <chrono>
#include <memory>

#include "../itc.h"

namespace ns_CallStaticDemo {

    static const std::string THREAD = "thread";

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

    void func1() {
        // wait a little to syncronize outputs
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        std::cout << itc::currentThreadName() << ": " << "func1()" << std::endl;
    }

    void func2(int i){ 
        std::cout << itc::currentThreadName() << ": " << "func2(" << i << ")" << std::endl; 
    }

    void func3(std::string s, int i, float f) { 
        std::cout << itc::currentThreadName() << ": " << "func3(" << s << ", " << i << ", " << f << ")" << std::endl;
    }
    
    void func4(std::shared_ptr<A> a) {
        std::cout << itc::currentThreadName() << ": " << "func4(" << a << ")" << std::endl;
    }
}

class CallStaticDemo
{
public:

    void run();
};

DECLARE_STATIC_CALL (STATIC_CALL_func1, ns_CallStaticDemo::THREAD, ns_CallStaticDemo::func1)
DECLARE_STATIC_CALL (STATIC_CALL_func2, ns_CallStaticDemo::THREAD, ns_CallStaticDemo::func2, int)
DECLARE_STATIC_CALL (STATIC_CALL_func3, ns_CallStaticDemo::THREAD, ns_CallStaticDemo::func3, std::string, int, float)
DECLARE_STATIC_CALL (STATIC_CALL_func4, ns_CallStaticDemo::THREAD, ns_CallStaticDemo::func4, std::shared_ptr<ns_CallStaticDemo::A>)

void CallStaticDemo::run()
{
    itc::createEventLoop(ns_CallStaticDemo::THREAD);

    itc::invoke(STATIC_CALL_func1::CallStatic());
    itc::invoke(STATIC_CALL_func2::CallStatic(STATIC_CALL_func2::Params(5)));
    itc::invoke(STATIC_CALL_func3::CallStatic(STATIC_CALL_func3::Params("HELLO", 42, 5.5f)));
    itc::invoke(STATIC_CALL_func4::CallStatic(STATIC_CALL_func4::Params(std::make_shared<ns_CallStaticDemo::A>(33, "Hello A"))));
}