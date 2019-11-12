/** @file itc.c
 *  @brief Implementation of public intrerface function for using ITC.
 *
 *  This contains functions to create and start EventLoops,
 *  to invoke methods in event loops, runtimers, etc.
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#include "../itc.hpp"

#include <iostream>

#include "Dispatcher.hpp"
#include "EventLoop.hpp"

namespace itc {

    bool invoke(const std::string& threadName, std::shared_ptr<_private::ICallable> call) {
        bool result = false;

        //std::cout << "itc::invoke" << std::endl;
        std::shared_ptr<_private::EventLoop> thread = _private::Dispatcher::getInstance().getThreadByName(threadName);

        if (thread == nullptr) {
            std::cerr << "Dispatcher::postMsg ERROR thread == nullptr " << std::endl;
        } else {
            //std::cout << "itc::invoke push" << std::endl;
            thread->push(call);
            result = true;
        }
        return result; 
    }

    Timer& timer(const std::string& threadName, std::shared_ptr<_private::ICallable> call, 
        std::chrono::milliseconds period, bool repeating) {

        std::shared_ptr<_private::EventLoop> thread = _private::Dispatcher::getInstance().getThreadByName(threadName);
        return thread->addTimer(call, period, repeating);
    }

    long long getTimeFromStart() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - gAppStartTime).count();
    }

    void createEventLoop(const std::string& threadName) {
        std::shared_ptr<_private::EventLoop> eventLoop = std::make_shared<_private::EventLoop>(threadName);
        _private::Dispatcher::getInstance().registerEventLoop(eventLoop);
    }

    bool invoke(const itc::_private::CallBinder& callBinder)
    {
        return invoke(callBinder.getThreadName(), callBinder.getCallable());
    }

    Timer& timer(const itc::_private::CallBinder& callBinder, std::chrono::milliseconds period, bool repeating) {
        return timer(callBinder.getThreadName(), callBinder.getCallable(), period, repeating);
    }    

    void deleteTimer(const std::string& threadName, const Timer& timer) {
        
        std::shared_ptr<_private::EventLoop> eventLoop = _private::Dispatcher::getInstance().getThreadByName(threadName);
        return eventLoop->removeTimer(timer);
    }

    int getLastTimerId() {
        return _private::Dispatcher::getInstance().getThreadById(std::this_thread::get_id())->getLastTimerId();
    }

    const static std::string MAIN_THREAD_NAME = "_UNKNOWN_";
    const std::string& currentThreadName() {
        std::shared_ptr<_private::EventLoop> eventLoop = _private::Dispatcher::getInstance().getThreadById(std::this_thread::get_id());
        return eventLoop ? eventLoop->getThreadName() : MAIN_THREAD_NAME;
    }
}