/** @file EventLoop.h
 *  @brief EventLoop class declaration
 *
 *  Provides event loop functionality, wrap threads for processing async events
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <list>

#include "Timer.hpp"

namespace itc {
    namespace _private {

        class Event;
        class Call;

        class EventLoop 
        {
        public:
            EventLoop(const std::string& threadName);
            ~EventLoop();

            bool createThread();
            void exitThread();

            std::thread::id getThreadId() const;
            const std::string& getThreadName() const;
            static std::thread::id getCurrentThreadId();
            inline int getLastTimerId() { return mLastTimerId; }
            
            void push(const ICallable* call);
            Timer& addTimer(const ICallable* call, std::chrono::milliseconds period, bool repeating);
            void removeTimer(const Timer& timer);
            void bringNextToFront();
            
        private:
            EventLoop(const EventLoop&);
            EventLoop& operator=(const EventLoop&);

            void run();
            
            bool mbStop;
            std::thread* mThread;
            const std::string mThreadName;
            std::queue<Event*> mEvents;
            std::list<Timer> mTimers;
            static int mNextTimerId;
            int mLastTimerId;
            std::mutex mMutex;
            std::condition_variable mCV;     
        };

    }
}