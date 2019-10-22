/** @file Dispatcher.cpp
 *  @brief Dispatcher class implementation
 *
 *  Contains collection of created event loops and dispatch invoced event to event loops
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#include "Dispatcher.h"
#include <iostream>
#include <algorithm>
#include <thread>

#include "EventLoop.h"

namespace itc {
    namespace _private {

        Dispatcher* Dispatcher::mpInstance = nullptr;

        Dispatcher::Dispatcher()
        : mThreads()
        {}

        void Dispatcher::registerThread(EventLoop* thread) {

            std::lock_guard<std::recursive_mutex> lock (gMutex);
            const std::thread::id& id = thread->getThreadId();
            mThreads[id] = thread;
            std::cout << "Dispatcher::registerThread " << id << " " << thread->getThreadName() << " size=" << mThreads.size() << std::endl;
            //printThreads();
        }

        void Dispatcher::printThreads() const
        {
            for (auto thread: mThreads) {
                std::cout << thread.first << ':' << thread.second->getThreadName() << " " << thread.second->getThreadId() << std::endl;
            }        
        }

        EventLoop* Dispatcher::getThreadById(const std::thread::id& id) const {
            std::lock_guard<std::recursive_mutex> lock (gMutex);
            if (mThreads.count(id) > 0) {
                return mThreads.at(id);
            } else {
                //std::cerr << "Dispatcher::getThreadById ERROR id = " << id << std::endl;
                //printThreads();
                return nullptr;
            }
        }       

        EventLoop* Dispatcher::getThreadByName(const std::string& name) const {
            std::lock_guard<std::recursive_mutex> lock (gMutex);

            auto it = std::find_if( mThreads.begin(), mThreads.end(), [name](const std::pair<std::thread::id, EventLoop*>& p) {
                return p.second->getThreadName() == name;
            });
            
            if (it != mThreads.end()) {
                return it->second;
            } else {
                return nullptr;
            }
        }
    }
}