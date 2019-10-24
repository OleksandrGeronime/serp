/** @file itc.h
 *  @brief Public intrerface function for using ITC.
 *
 *  This contains functions to create and start EventLoops,
 *  to invoke methods in event loops, runtimers, etc.
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include "_private/Call.h"
#include "_private/Timer.h"
#include "_private/CallBinder.h"

// Declares global alias for global or static function which could be called on specified thread from any thread in application
// * CONNECTOR - allias for call, should be UNIQUE for all application
// * THREAD - thred where funciton will be calling, should be created and registered in Dispatcher by void createEventLoop(const std::string& threadName);
// * METHOD - full name of funciton with namespaces and class names, funcition shold be public and return void
// * ... funciton argument types
#define DECLARE_STATIC_CALL(CONNECTOR, THREAD, METHOD, ...) \
namespace CONNECTOR { \
    typedef std::tuple<__VA_ARGS__> Params; \
    class CallStatic : public itc::_private::CallBinder { \
        public: \
        CallStatic(Params params = Params()) \
        : CallBinder(THREAD, new itc::CallStatic<__VA_ARGS__>(METHOD, params)) \
        { \
            std::cout << itc::currentThreadName() << " ---CALL---> " << THREAD << ": " << #METHOD << params << std::endl; \
        } \
    }; \
}

// Declares global alias for member function which could be called on specified thread from any thread in application
// * CONNECTOR - allias for call, should be UNIQUE for all application
// * THREAD - thred where funciton will be calling, should be created and registered in Dispatcher by void createEventLoop(const std::string& threadName);
// * CLASS - full name of class where function declared with namespaces and class names
// * METHOD - name of funciton without namespaces and class names, funcition shold be public and return void
// * ... funciton argument types
#define DECLARE_CALL(CONNECTOR, THREAD, CLASS, METHOD, ...) \
namespace CONNECTOR { \
    auto METHOD = std::mem_fn(static_cast<void (CLASS::*)(__VA_ARGS__)>(&CLASS::METHOD)); \
    typedef std::tuple<__VA_ARGS__> Params; \
    class Call : public itc::_private::CallBinder \
    { \
    public: \
        Call(CLASS* context, Params params = Params()): CallBinder(THREAD, new itc::Call<CLASS, ## __VA_ARGS__>(context, METHOD, params)) \
        { \
            std::cout << itc::currentThreadName() << " ---CALL---> " << THREAD << ": " << #CLASS << "::" << #METHOD << params << std::endl; \
        } \
    }; \
}

// Declares global alias for member function which will be called thread where it invoked asynchronously
// * CONNECTOR - allias for event, should be UNIQUE for all application
// * CLASS - full name of class where function declared with namespaces and class names
// * METHOD - name of funciton without namespaces and class names, funcition shold be public and return void
// * ... funciton argument types
#define DECLARE_EVENT(CONNECTOR, CLASS, METHOD, ...) \
namespace CONNECTOR { \
    auto METHOD = std::mem_fn(static_cast<void (CLASS::*)(__VA_ARGS__)>(&CLASS::METHOD)); \
    typedef std::tuple<__VA_ARGS__> Params; \
    class Event : public itc::_private::CallBinder \
    { \
    public: \
        Event(CLASS* context, Params params = Params()): CallBinder(itc::currentThreadName(), new itc::Call<CLASS, ## __VA_ARGS__>(context, METHOD, params)) \
        { \
            std::cout << " ---EVENT---> " << itc::currentThreadName() << ": " << #CLASS << "::" << #METHOD << params << std::endl; \
        } \
    }; \
}

// Declares global alias for request which could be sent to member function in specified thread and result value of this funciton
// received in member funciton in curretn thread
// * CONNECTOR - allias for request, should be UNIQUE for all application
// * THREAD - thred where funciton will be calling, should be created and registered in Dispatcher by void createEventLoop(const std::string& threadName);
// * CLASS_RESPONSE - full name of class where response will be received 
// * METHOD_RESPONSE - name of method in CLASS_RESPONSE where result will be received, should have only one argument with type RET
// * RET - type of result value
// * CLASS_REQUEST - full name of class where request will be processed
// * METHOD_REQUEST - name of funciton in class METHOD_REQUEST wich will process request, funcition shold be public and return value of type RET
// * ... METHOD_REQUEST argument types
#define DECLARE_REQUEST(CONNECTOR, THREAD, CLASS_RESPONSE, METHOD_RESPONSE, RET, CLASS_REQUEST, METHOD_REQUEST, ...) \
namespace CONNECTOR { \
    auto onRequestSum = std::mem_fn(static_cast<RET (CLASS_REQUEST::*)(__VA_ARGS__)>(&CLASS_REQUEST::METHOD_REQUEST)); \
    auto onResponseSum = std::mem_fn(static_cast<void (CLASS_RESPONSE::*)(RET)>(&CLASS_RESPONSE::METHOD_RESPONSE)); \
    typedef std::tuple<__VA_ARGS__> Params; \
    class Request : public itc::_private::CallBinder \
    { \
    public: \
        Request(CLASS_REQUEST* contextRequest, Params params, CLASS_RESPONSE* contextResponse) \
            : CallBinder(THREAD, new itc::Request<CLASS_REQUEST, CLASS_RESPONSE, RET, ## __VA_ARGS__>(contextRequest, contextResponse, onRequestSum, onResponseSum, params)) \
            { \
                std::cout << itc::currentThreadName() << " ---REQUEST---> " << THREAD << ": " \
                << #CLASS_REQUEST << "::" << #METHOD_REQUEST << params << " RESP -> " << #CLASS_RESPONSE << "::" << #METHOD_RESPONSE << std::endl; \
            } \
    }; \
}

namespace itc {
    // Global variable storing time point of application start
    static const std::chrono::system_clock::time_point gAppStartTime = std::chrono::system_clock::now();

    // Retruns time duration in milliseconds from application start
    long long getTimeFromStart();

    // Create thread and register in Dispatcher, threadName should be UNIQE for all application
    void createEventLoop(const std::string& threadName);

    // Invoke ICallable object on binded thread.
    // * callBinder - binder ICallable object types to thread should be declared using one of macro:
    // DECLARE_STATIC_CALL, DECLARE_CALL, DECLARE_EVENT, DECLARE_REQUEST
    bool invoke(const itc::_private::CallBinder& callBinder);

    // Create timer and register it in current(or other) thread
    // * callBinder - binder ICallable object type to to thread, recomended to use DECLARE_EVENT only to create callBinder for timers
    // * period - time period for timer elapsing
    // * repeating - if true timer will be repeatable recurrently
    // * return - ref to timer object
    Timer& timer(const itc::_private::CallBinder& callBinder, std::chrono::milliseconds period, bool repeating);

    // Delete timer from thread
    // * threadName - name of thread where timet to be deleted registered
    // * timer - ref to timer object to be deleted
    void deleteTimer(const std::string& threadName, const Timer& timer);
    
    // Returns id of last elapsed timer
    // Each timer have id unique for all application, this function could be used for defining which timer instance was elapsed in on timer elapsed funciton
    int getLastTimerId();
    
    // Returns name of current thread
    const std::string& currentThreadName();
}