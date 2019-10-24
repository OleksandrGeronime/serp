/** @file CallDemo.h
 *  @brief Demo call functionality
 *
 *  Call could be assigned to any public memember function and invoked in different thread. 
 *  In this demo one thread created and 4 different calls made from main to this thread.
 *  Each call execute one of 4 member functions with dfferent arguments lists.
 * 
 *  @date 2019
 *  @author Alexander Geronime 
 */

#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <memory>

#include "../itc.h"

namespace ns_CallDemo {
    static const std::string THREAD = "CallDemo_thread";

    struct A 
    {
        void func1()
        {
            // wait a little to syncronize outputs
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            std::cout << itc::currentThreadName() << ": " << "func1()" << std::endl;
        }

        void func2(int i)
        { 
            std::cout << itc::currentThreadName() << ": " << "func2(" << i << ")" << std::endl; 
        }

        void func3(std::string s, int i, float f)
        { 
            std::cout << itc::currentThreadName() << ": " << "func3(" << s << ", " << i << ", " << f << ")" << std::endl;
        }

        void func4(std::shared_ptr<ns_CallDemo::A> pA);

        A(int i, std::string s): mI(i), mS(s) {}
        int mI;
        std::string mS;
    };

    std::ostream& operator<<(std::ostream &o, std::shared_ptr<A> a)
    {
        return o << "{i=" << a->mI << ", s=" << a->mS << "}";
    }

    void A::func4(std::shared_ptr<ns_CallDemo::A> pA)
    {
        std::cout << itc::currentThreadName() << ": " << "func4(" << pA << ")" << std::endl;
    }
}

DECLARE_CALL(CALL_func1, ns_CallDemo::THREAD, ns_CallDemo::A, func1)
DECLARE_CALL(CALL_func2, ns_CallDemo::THREAD, ns_CallDemo::A, func2, int)
DECLARE_CALL(CALL_func3, ns_CallDemo::THREAD, ns_CallDemo::A, func3, std::string, int, float)
DECLARE_CALL(CALL_func4, ns_CallDemo::THREAD, ns_CallDemo::A, func4, std::shared_ptr<ns_CallDemo::A>)

class CallDemo
{
public:
    void run();
};

void CallDemo::run() 
{
    itc::createEventLoop(ns_CallDemo::THREAD);

    auto a = std::make_shared<ns_CallDemo::A>(123, "HELLO A");

    itc::invoke(CALL_func1::Call(a.get()));
    itc::invoke(CALL_func2::Call(a.get(), CALL_func2::Params(42)));
    itc::invoke(CALL_func3::Call(a.get(), CALL_func3::Params("Demo", 5, 3.14f)));
    itc::invoke(CALL_func4::Call(a.get(), CALL_func4::Params(a)));
}