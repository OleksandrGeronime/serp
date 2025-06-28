/** @file CallBinder.h
 *  @brief Bind thread and callable object
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include <string>
#include <memory>

namespace itc {
    namespace _private {

        class ICallable;

        class CallBinder {
        public:
            CallBinder(const std::string& threadName, std::shared_ptr<itc::_private::ICallable> pCallable) 
                : mpCallable(pCallable)
                , mThreadName(threadName)
                {}
            virtual ~CallBinder() {};
            inline std::shared_ptr<itc::_private::ICallable> getCallable() const { return mpCallable; }
            inline const std::string& getThreadName() const { return mThreadName; }
        protected:
            std::shared_ptr<itc::_private::ICallable> mpCallable; 
            const std::string& mThreadName; 
        };
    }
}