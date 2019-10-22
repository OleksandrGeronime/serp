/** @file CallDemo.h
 *  @brief Demo call functionality
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#pragma once

#include <iostream>
#include <string>
#include <chrono>

#include "../itc.h"

namespace ns_CallDemo {
    static const std::string THREAD_1 = "thread_1";

    struct A 
    {
        void func1(){
            // wait a little to syncronize outputs
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            std::cout << itc::currentThreadName() << ": " << "func1()" << std::endl;
        };
        void func2(int i){ 
            std::cout << itc::currentThreadName() << ": " << "func2(" << i << ")" << std::endl; 
        };
        void func3(std::string s, int i, float f) { 
            std::cout << itc::currentThreadName() << ": " << "func3(" << s << ", " << i << ", " << f << ")" << std::endl;
        };
        void func4(A* pA);
    };
}

DECLARE_CALL(CALL_func1, ns_CallDemo::THREAD_1, ns_CallDemo::A, func1)
DECLARE_CALL(CALL_func2, ns_CallDemo::THREAD_1, ns_CallDemo::A, func2, int)
DECLARE_CALL(CALL_func3, ns_CallDemo::THREAD_1, ns_CallDemo::A, func3, std::string, int, float)
DECLARE_CALL(CALL_func4, ns_CallDemo::THREAD_1, ns_CallDemo::A, func4, ns_CallDemo::A*)

void ns_CallDemo::A::func4(A* pA){
    static int i = 0;
    std::cout << itc::currentThreadName() << ": " << "func4() i = " << i++ << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    itc::invoke(CALL_func4::Call(this, CALL_func4::Params(this)));
};

class CallDemo
{
public:
    void run();
};

void CallDemo::run() 
{
    itc::createEventLoop(ns_CallDemo::THREAD_1);

    ns_CallDemo::A a;

    itc::invoke(CALL_func1::Call(&a));
    itc::invoke(CALL_func2::Call(&a, CALL_func2::Params(42)));
    itc::invoke(CALL_func3::Call(&a, CALL_func3::Params("Demo", 5, 3.14f)));

    // run recursive async call
    itc::invoke(CALL_func4::Call(&a, CALL_func4::Params(&a)));
}