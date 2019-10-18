/** @file Timer.cpp
 *  @brief Timer class implementation
 *
 *  Provides timers functionality
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#include "../Timer.h"

#include "ICallable.h"
#include "Event.h"
#include "Dispatcher.h"
#include "EventLoop.h"

namespace itc {

    Timer::Timer(const _private::ICallable* callable, std::chrono::milliseconds period, bool repeating, int timerId)
        : mpEvent(new _private::Event(callable))
        , mPeriod(period)
        , mbRepeating(repeating)
        , mbStarted(false)
        , mStartedTime()
        , mTimerId(timerId)
    {}

    void Timer::call() {
        mpEvent->getCallable()->call();
    }

    void Timer::start() {
        mStartedTime = std::chrono::system_clock::now();
        mbStarted = true;
        _private::Dispatcher::getInstance()->getThreadById(std::this_thread::get_id())->bringNextToFront();
    };

    void Timer::stop() { 
        mbStarted = false; 
        _private::Dispatcher::getInstance()->getThreadById(std::this_thread::get_id())->bringNextToFront();
    };

}