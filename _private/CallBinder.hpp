/** @file CallBinder.h
 *  @brief Bind thread and callable object
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include <string>

namespace itc {
    namespace _private {

        class ICallable;

        class CallBinder {
        public:
            CallBinder(const std::string& threadName, itc::_private::ICallable* pCallable) 
                : mpCallable(pCallable)
                , mThreadName(threadName)
                {}
            virtual ~CallBinder() {};
            inline const itc::_private::ICallable* getCallable() const { return mpCallable; }
            inline const std::string& getThreadName() const { return mThreadName; }
        protected:
            const itc::_private::ICallable* mpCallable; 
            const std::string& mThreadName; 
        };
    }
}