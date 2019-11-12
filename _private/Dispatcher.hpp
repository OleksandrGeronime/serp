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
#include <memory>
namespace itc {
    namespace _private {

        class EventLoop;
        static std::recursive_mutex gMutex;

        class Dispatcher
        {
        public:
            static Dispatcher& getInstance() {
                static Dispatcher instance;
                return instance;    
            }

            void registerEventLoop(std::shared_ptr<EventLoop> eventLoop);

            void printThreads() const;

            std::shared_ptr<EventLoop> getThreadByName(const std::string& name) const;
            std::shared_ptr<EventLoop> getThreadById(const std::thread::id& id) const;
            
        private:
            Dispatcher();
            std::map<std::thread::id, std::shared_ptr<EventLoop>> mThreads;
        };

}
}