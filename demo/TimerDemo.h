/** @file TimerDemo.h
 *  @brief Demo timer functionality
 * 
 *  There are two threds in this sample
 *  In first thread one timers started repeateadly and elapse each 3 seconds.
 *  In second thread 2 timers started for 500ms and 2s repeateadly. 
 *  Both of them call the same method. Which timer made call could be defined using itc::getLastTimerId()
 *  First will be stopped after 5 elapsing, second - after 6.
 *  
 *  @date 2019
 *  @author Alexander Geronime 
 */

#pragma once

#include <iostream>
#include <string>
#include <chrono>

#include "../itc.h"

namespace ns_TimerDemo {
    static const std::string THREAD_1 = "thread_1";
    static const std::string THREAD_2 = "thread_2";

    class A1
    {
    public:
        void startTimer();
        void onTimerEvent();
    };

    class A2
    {
    public:
        void startTimer();
        void onTimerEvent();

        itc::Timer* mpT1;
        itc::Timer* mpT2;
    };
}

DECLARE_CALL(CALL_A1_startTimer, ns_TimerDemo::THREAD_1, ns_TimerDemo::A1, startTimer)
DECLARE_CALL(CALL_A1_onTimerEvent, ns_TimerDemo::THREAD_1, ns_TimerDemo::A1, onTimerEvent)
DECLARE_CALL(CALL_A2_startTimer, ns_TimerDemo::THREAD_2, ns_TimerDemo::A2, startTimer)
DECLARE_CALL(CALL_A2_onTimerEvent, ns_TimerDemo::THREAD_2, ns_TimerDemo::A2, onTimerEvent)

void ns_TimerDemo::A1::startTimer()
{
    std::cout << itc::currentThreadName() << ": " << "A1::startTimer()" << std::endl;
    auto& t =  itc::timer(CALL_A1_onTimerEvent::Call(this), std::chrono::seconds(3), true);
    std::cout << itc::currentThreadName() << ": " << "ns_TimerDemo::A1::startTimer() t timerId=" << t.getId() << std::endl;
    t.start();
}
void ns_TimerDemo::A1::onTimerEvent()
{
    std::cout << itc::currentThreadName() << ": " << "A1::onTimerEvent() timerId=" << itc::getLastTimerId() << ", time_from_app_start:" << itc::getTimeFromStart() << std::endl;
}
void ns_TimerDemo::A2::startTimer()
{
    std::cout << itc::currentThreadName() << ": " << "A1::startTimer()" << std::endl;
    
    mpT1 = &itc::timer(CALL_A2_onTimerEvent::Call(this), std::chrono::milliseconds(500), true);
    std::cout << itc::currentThreadName() << ": " << "ns_TimerDemo::A2::startTimer() mpT1 timerId=" << mpT1->getId() << std::endl;
    mpT1->start();
    
    mpT2 = &itc::timer(CALL_A2_onTimerEvent::Call(this), std::chrono::seconds(1), true);
    std::cout << itc::currentThreadName() << ": " << "ns_TimerDemo::A2::startTimer() mpT2 timerId=" << mpT2->getId() << std::endl;
    mpT2->start();
}
void ns_TimerDemo::A2::onTimerEvent()
{
    std::cout << itc::currentThreadName() << ": " << "A2::onTimerEvent() timerId=" << itc::getLastTimerId() << ", time_from_app_start:" << itc::getTimeFromStart() << std::endl;
    
    static int mpT1_counter = 0;
    static int mpT2_counter = 0;
    
    int id = itc::getLastTimerId();
    if (id == mpT1->getId()) {
        if (mpT1_counter == 5) {
            std::cout << itc::currentThreadName() << ": " << "A2::onTimerEvent() STOP mpT1 timerId=" << id << std::endl;
            mpT1->stop();
        }
        mpT1_counter++;
    } else if (id == mpT2->getId()) {
        if (mpT2_counter == 6) {
            std::cout << itc::currentThreadName() << ": " << "A2::onTimerEvent() STOP mpT2 timerId=" << id << std::endl;
            mpT2->stop();
        }
        mpT2_counter++;
    }
}

class TimerDemo
{
public:
    void run();
};

void TimerDemo::run() 
{
    itc::createEventLoop(ns_TimerDemo::THREAD_1);
    itc::createEventLoop(ns_TimerDemo::THREAD_2);

    ns_TimerDemo::A1* a1 = new ns_TimerDemo::A1();
    ns_TimerDemo::A2* a2 = new ns_TimerDemo::A2();

    itc::invoke(CALL_A1_startTimer::Call(a1));
    itc::invoke(CALL_A2_startTimer::Call(a2));
}