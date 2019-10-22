/** @file EventLoop.cpp
 *  @brief EventLoop class implementation
 *
 *  Provides event loop functionality, wrap threads for processing async events
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#include "EventLoop.h"

//#include <iostream>
#include <algorithm>
#include <chrono>

#include "Event.h"
#include "../Call.h"

static const int LOG_ENABLE = 0;

namespace itc {
    namespace _private {

        int EventLoop::mNextTimerId = 0;

        EventLoop::EventLoop(const std::string& threadName) 
        : mbStop(false)
        , mThread(nullptr)
        , mThreadName(threadName)
        , mEvents()
        , mTimers()
        , mLastTimerId(0)
        , mMutex()
        , mCV()        
        {
        }

        EventLoop::~EventLoop()
        {
            exitThread();
        }

        bool EventLoop::createThread()
        {
            if (!mThread)
                mThread = new std::thread(&EventLoop::run, this);    
            return true;
        }

        std::thread::id EventLoop::getThreadId() const
        {
            return mThread->get_id();
        }

        const std::string& EventLoop::getThreadName() const
        {
            return mThreadName;
        }

        std::thread::id EventLoop::getCurrentThreadId()
        {
            return std::this_thread::get_id();
        }

        void EventLoop::push(const ICallable* call)
        {
            Event* event = new Event(call);

            std::unique_lock<std::mutex> lock(mMutex);
            mEvents.push(event);

            if (LOG_ENABLE) std::cout << getThreadName() << " EventLoop::push" << std::endl;
            mCV.notify_one();
        }

        void EventLoop::bringNextToFront() 
        {
            std::unique_lock<std::mutex> lock(mMutex);
            if (LOG_ENABLE) std::cout << "EventLoop::bringNextToFront" << std::endl;

            if (mTimers.size() > 1) {
                auto frontTimer = mTimers.begin();
                auto nextTimer = std::min_element(mTimers.begin(), mTimers.end(), 
                    []( const Timer &a, const Timer &b )
                    {
                        return a.isStarted() && a.getStartedTime() + a.getPeriod() < b.getStartedTime() + b.getPeriod();
                    });

                if (LOG_ENABLE) std::cout << "EventLoop::bringNextToFront " << frontTimer->getId() << " " << nextTimer->getId() << " " << mTimers.begin()->getId() << std::endl;
                if (nextTimer != mTimers.begin()) {
                    mTimers.splice(mTimers.begin(), mTimers, nextTimer, std::next(nextTimer));
                }
                if (LOG_ENABLE) std::cout << "EventLoop::bringNextToFront " << frontTimer->getId() << " " << nextTimer->getId() << " " << mTimers.begin()->getId() << std::endl;
            }

            mCV.notify_one();
        }

        Timer& EventLoop::addTimer(const ICallable* call, std::chrono::milliseconds period, bool repeating)
        {
            mTimers.emplace_back(call, period, repeating, mNextTimerId);
            Timer& result = mTimers.back();
            ++mNextTimerId;

            bringNextToFront();

            if (LOG_ENABLE) std::cout << getThreadName() << " EventLoop::addTimer" << std::endl;
            mCV.notify_one();
            return result;
        }

        void EventLoop::removeTimer(const Timer& timer)
        {
            std::unique_lock<std::mutex> lock(mMutex);
            mTimers.remove(timer);
            
            bringNextToFront();

            if (LOG_ENABLE)  std::cout << getThreadName() << " EventLoop::removeTimer" << std::endl;
            mCV.notify_one();
        }

        void EventLoop::exitThread()
        {
            if (!mThread)
                return;

            Event* event = new Event(EventType::SYSTEM, EventPriority::HIGHEST, 0);
            {
                std::lock_guard<std::mutex> lock(mMutex);
                mEvents.push(event);                
                mCV.notify_one();
            }

            mThread->join();
            delete mThread;
            mThread = 0;
        }

        void EventLoop::run()
        {    
            while (true)
            {
                if (LOG_ENABLE) std::cout << getThreadName() << " loop" << std::endl;

                const Event* event = nullptr;
                bool bDeleteCallable = true;
                {
                    // std::contidion_variable::wait_for with max period not wait, so will use just some big period value for wait if no timers 
                    //auto timeToNextTimer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::duration::max());
                    auto timeToNextTimer = std::chrono::milliseconds(1000000);
                    if (!mTimers.empty()) {
                        Timer& timer = mTimers.front();
                        if (timer.isStarted()) {
                            if (LOG_ENABLE) std::cout << getThreadName() << " front timer started" << std::endl;
                            auto now = std::chrono::system_clock::now();
                            auto dur = now - timer.getStartedTime();
                            timeToNextTimer = timer.getPeriod() - std::chrono::duration_cast<std::chrono::milliseconds>(dur);
                            if (timeToNextTimer <= std::chrono::microseconds(0)) {
                                event = timer.getEvent();
                                mLastTimerId = timer.getId();
                                if (timer.isRepeating()) {
                                    timer.start();
                                    bDeleteCallable = false;
                                }
                                else {
                                    mTimers.remove(timer);
                                    bringNextToFront();
                                }
                            }
                        }
                        else { if (LOG_ENABLE) std::cout << getThreadName() << " front timer NOT started" << std::endl; } 
                    } 
                    else { if (LOG_ENABLE) std::cout << getThreadName() << " timers empty" << std::endl; }
                    if (LOG_ENABLE) std::cout << getThreadName() << " checked timer" << std::endl;

                    std::unique_lock<std::mutex> lock(mMutex);
                    if (LOG_ENABLE) std::cout << getThreadName() << " pass lock" << std::endl;

                    if (event == nullptr) {
                        if (mEvents.empty()) {
                            if (LOG_ENABLE) std::cout << getThreadName() << " wait_for " << timeToNextTimer.count() << std::endl;
                            mCV.wait_for(lock, timeToNextTimer);
                        }
                        
                        if (mEvents.empty())
                            continue;

                        event = mEvents.front();
                        mEvents.pop();
                    }
                }

                if (LOG_ENABLE)  std::cout << getThreadName() << " got event and call" << std::endl;
                const ICallable* callable = event->getCallable();
                callable->call();
                
                if (bDeleteCallable) {
                    delete event;
                }
            }
        }

    }
}