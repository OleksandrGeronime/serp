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
#include <condition_variable>
#include <queue>
#include <list>
#include <memory>

#include "Timer.hpp"

namespace itc {
    namespace _private {

        class Event;
        class Call;

        class EventLoop 
        {
        public:
            EventLoop(const std::string& threadName);
            ~EventLoop() = default;

            void exitThread();

            std::thread::id getThreadId() const;
            const std::string& getThreadName() const;
            static std::thread::id getCurrentThreadId();
            inline int getLastTimerId() { return mLastTimerId; }
            
            void push(std::shared_ptr<ICallable> call);
            Timer& addTimer(std::shared_ptr<ICallable> call, std::chrono::milliseconds period, bool repeating);
            void removeTimer(const Timer& timer);
            void bringNextToFront();
            
        private:
            EventLoop(const EventLoop&);
            EventLoop& operator=(const EventLoop&);

            void run();
            
            bool mbStop;
            std::thread::id mThreadId;
            const std::string mThreadName;
            std::queue<std::shared_ptr<Event>> mEvents;
            std::list<Timer> mTimers;
            static int mNextTimerId;
            int mLastTimerId;
            std::mutex mMutex;
            std::condition_variable mCV;     
        };

    }
}