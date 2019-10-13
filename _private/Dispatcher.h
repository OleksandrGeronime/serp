/** @file Dispatcher.h
 *  @brief Dispatcher class declaration
 *
 *  Contains collection of created event loops and dispatch invoced event to event loops
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include "EventLoop.h"
#include <map>
#include <mutex>
#include <string>

namespace itc {
    namespace _private {

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

            void printThreads() {
                for ( auto it = mThreads.begin(); it != mThreads.end(); it++ )
                {
                    std::cout << it->first  
                            << ':'
                            << it->second->getThreadName() << " " << it->second->getThreadId()
                            << std::endl ;
                }        
            }

            EventLoop* getThreadByName(const std::string& name);
			EventLoop* getThreadById(const std::thread::id& id);
            
        private:
            Dispatcher(){};

            
            
            static Dispatcher* mpInstance;
            std::map<std::thread::id, EventLoop*> mThreads;
        };

}
}