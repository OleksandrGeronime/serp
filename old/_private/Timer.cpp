/** @file Timer.cpp
 *  @brief Timer class implementation
 *
 *  Provides timers functionality
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#include <iostream>

#include "ICallable.hpp"
#include "Timer.hpp"
#include "Event.hpp"
#include "Dispatcher.hpp"
#include "EventLoop.hpp"

namespace itc {

    Timer::Timer(std::shared_ptr<_private::ICallable> callable, std::chrono::milliseconds period, bool repeating, int timerId)
        : mpEvent(new _private::Event(callable))
        , mPeriod(period)
        , mbRepeating(repeating)
        , mbStarted(false)
        , mStartedTime()
        , mTimerId(timerId)
    {
        //std::cout << "Timer" << std::endl;
    }

    Timer::~Timer()
    {
        //std::cout << "~Timer" << std::endl;
    }

    void Timer::call() {
        mpEvent->getCallable()->call();
    }

    void Timer::start() {
        mStartedTime = std::chrono::system_clock::now();
        mbStarted = true;
        _private::Dispatcher::getInstance().getThreadById(std::this_thread::get_id())->bringNextToFront();

        //std::cout << "Timer::start()" << std::endl;
    }

    void Timer::stop() {
        //std::cout << "Timer::stop()" << std::endl;
        mbStarted = false; 
        _private::Dispatcher::getInstance().getThreadById(std::this_thread::get_id())->bringNextToFront();
    }

}