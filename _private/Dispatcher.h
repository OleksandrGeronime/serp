/** @file Dispatcher.h
 *  @brief Dispatcher class declaration
 *
 *  Contains collection of created event loops and dispatch invoced event to event loops
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include <map>
#include <mutex>
#include <string>
#include <thread>

namespace itc {
    namespace _private {

        class EventLoop;
        static std::recursive_mutex gMutex;

        class Dispatcher
        {
        public:
            static Dispatcher* getInstance() {
                std::lock_guard<std::recursive_mutex> lock (gMutex);
                if (nullptr == mpInstance) {
                    mpInstance = new Dispatcher();
                }
                return mpInstance;    
            }

            void registerThread(EventLoop* thread);

            void printThreads() const;

            EventLoop* getThreadByName(const std::string& name) const;
            EventLoop* getThreadById(const std::thread::id& id) const;
            
        private:
            Dispatcher();

            static Dispatcher* mpInstance;
            std::map<std::thread::id, EventLoop*> mThreads;
        };

}
}