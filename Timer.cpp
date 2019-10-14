/** @file Timer.cpp
 *  @brief Timer class implementation
 *
 *  Provides timers functionality
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#include "Timer.h"
#include <thread>
#include "_private/ICallable.h"
#include "_private/Event.h"
#include "_private/Dispatcher.h"
#include "_private/EventLoop.h"

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