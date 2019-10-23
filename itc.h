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

#include "Call.h"
#include "Timer.h"
#include "_private/CallBinder.h"
#include <iostream>

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
    static std::chrono::system_clock::time_point gAppStartTime = std::chrono::system_clock::now();
    long long getTimeFromStart();

    void createEventLoop(const std::string& threadName);
    bool invoke(const itc::_private::CallBinder& callBinder);
    Timer& timer(const itc::_private::CallBinder& callBinder, std::chrono::milliseconds period, bool repeating);
    void deleteTimer(const std::string& threadName, const Timer& timer);
    int getLastTimerId();
    const std::string& currentThreadName();
}