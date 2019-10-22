/** @file CallStaticDemo.h
 *  @brief Demo static call functionality
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#include <iostream>
#include <string>

#include "../itc.h"

namespace ns_CallStaticDemo {

    static const std::string THREAD = "thread";

    struct A 
    {
        int i;
        std::string s;
    };

    void func1() {}
    void func2(int i) {}
    void func3(std::string& s, int i, float f) {}
    void func4(ns_CallStaticDemo::A& a) {}
}

//DECLARE_CALL(CALL_func1, )

class CallStaticDemo
{
public:

    void run();
};

void CallStaticDemo::run()
{
    itc::createEventLoop(ns_CallStaticDemo::THREAD);

    //itc::invoke(ns_CallStaticDemo::THREAD, new itc::CallStatic<int>(ns_CallStaticDemo::func1, 5));
}