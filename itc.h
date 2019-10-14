/** @file itc.h
 *  @brief Public intrerface function for using ITC.
 *
 *  This contains functions to create and start EventLoops,
 *  to invoke methods in event loops, runtimers, etc.
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include "_private/Dispatcher.h"
#include "_private/EventLoop.h"
#include "Call.h"
#include "Timer.h"

namespace itc {
    static void createEventLoop(const std::string& threadName) {
		std::lock_guard<std::recursive_mutex> lock(_private::gMutex);
        _private::EventLoop* thread = new _private::EventLoop(threadName);
        thread->createThread();
        _private::Dispatcher::getInstance()->registerThread(thread);
    }

    static bool invoke(const std::string& threadName, const _private::ICallable* call) {
        bool result = false;

		_private::EventLoop* thread = nullptr;
		{
			std::lock_guard<std::recursive_mutex> lock(_private::gMutex);
			thread = _private::Dispatcher::getInstance()->getThreadByName(threadName);
		}

        if (thread == nullptr) {
            std::cout << "Dispatcher::postMsg ERROR thread == nullptr " << std::endl;
        } else {
            thread->push(call);
            result = true;
        }
        return result; 
    }

	static Timer& createTimer(const std::string& threadName, const _private::ICallable* call, 
		std::chrono::milliseconds period, bool repeating) {

		_private::EventLoop* thread = nullptr;
		{
			std::lock_guard<std::recursive_mutex> lock(_private::gMutex);
			thread = _private::Dispatcher::getInstance()->getThreadByName(threadName);
		}

		return thread->addTimer(call, period, repeating);
	}

    static void deleteTimer(const std::string& threadName, const Timer& timer) {
		
		_private::EventLoop* thread = nullptr;
		{
			std::lock_guard<std::recursive_mutex> lock(_private::gMutex);
			thread = _private::Dispatcher::getInstance()->getThreadByName(threadName);
		}
		
		return thread->removeTimer(timer);
	}

	static inline int getLastTimerId() {
		std::lock_guard<std::recursive_mutex> lock(_private::gMutex);
		return _private::Dispatcher::getInstance()->getThreadById(std::this_thread::get_id())->getLastTimerId();
	}

	static inline const std::string& currentThreadName() {
		std::lock_guard<std::recursive_mutex> lock(_private::gMutex);
		return _private::Dispatcher::getInstance()->getThreadById(std::this_thread::get_id())->getThreadName();
	}
}