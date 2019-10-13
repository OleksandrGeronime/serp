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
#include "Call.h"
#include "_private/Timer.h"

namespace itc {
    static void createEventLoop(const std::string& threadName) {
        _private::EventLoop* thread = new _private::EventLoop(threadName);
        thread->createThread();
        _private::Dispatcher::getInstance()->registerThread(thread);
    }

    static bool invoke(const std::string& threadName, const _private::ICallable* call) {
        bool result = false;
        std::lock_guard<std::recursive_mutex> lock (_private::gMutex);
        _private::EventLoop* thread = _private::Dispatcher::getInstance()->getThreadByName(threadName);
        if (thread == nullptr) {
            std::cout << "Dispatcher::postMsg ERROR thread == nullptr " << std::endl;
        } else {
            thread->push(call);
            result = true;
        }
        return result; 
    }

	static inline _private::ITimer& createTimer(const std::string& threadName, const _private::ICallable* call, 
		std::chrono::milliseconds period, bool repeating) {
		return _private::Dispatcher::getInstance()->getThreadByName(threadName)->addTimer(call, period, repeating);
	}

	static inline int getLastTimerId() {
		return _private::Dispatcher::getInstance()->getThreadById(std::this_thread::get_id())->getLastTimerId();
	}

	static inline const std::string& currentThreadName() {
		return _private::Dispatcher::getInstance()->getThreadById(std::this_thread::get_id())->getThreadName();
	}
}