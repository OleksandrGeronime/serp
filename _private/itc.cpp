/** @file itc.c
 *  @brief Implementation of public intrerface function for using ITC.
 *
 *  This contains functions to create and start EventLoops,
 *  to invoke methods in event loops, runtimers, etc.
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#include "../itc.h"

#include <iostream>

#include "Dispatcher.h"
#include "EventLoop.h"

namespace itc {

    void createEventLoop(const std::string& threadName) {
        _private::EventLoop* thread = new _private::EventLoop(threadName);
        thread->createThread();
        _private::Dispatcher::getInstance()->registerThread(thread);
    }

    bool invoke(const itc::_private::CallBinder& callBinder)
    {
        return invoke(callBinder.getThreadName(), callBinder.getCallable());
    }

    bool invoke(const std::string& threadName, const _private::ICallable* call) {
        bool result = false;

        //std::cout << "itc::invoke" << std::endl;
        _private::EventLoop* thread = _private::Dispatcher::getInstance()->getThreadByName(threadName);

        if (thread == nullptr) {
            std::cerr << "Dispatcher::postMsg ERROR thread == nullptr " << std::endl;
        } else {
            //std::cout << "itc::invoke push" << std::endl;
            thread->push(call);
            result = true;
        }
        return result; 
    }

    Timer& createTimer(const std::string& threadName, const _private::ICallable* call, 
        std::chrono::milliseconds period, bool repeating) {

        _private::EventLoop* thread = _private::Dispatcher::getInstance()->getThreadByName(threadName);
        return thread->addTimer(call, period, repeating);
    }

    void deleteTimer(const std::string& threadName, const Timer& timer) {
        
        _private::EventLoop* thread = _private::Dispatcher::getInstance()->getThreadByName(threadName);
        return thread->removeTimer(timer);
    }

    int getLastTimerId() {
        return _private::Dispatcher::getInstance()->getThreadById(std::this_thread::get_id())->getLastTimerId();
    }

    const static std::string MAIN_THREAD_NAME = "_main_";
    const std::string& currentThreadName() {
        _private::EventLoop* pThread = _private::Dispatcher::getInstance()->getThreadById(std::this_thread::get_id());
        return pThread ? pThread->getThreadName() : MAIN_THREAD_NAME;
    }
}