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

#define DECLARE_REQUEST(CONNECTOR, THREAD, CLASS_RESPONSE, METHOD_RESPONSE, RET, CLASS_REQUEST, METHOD_REQUEST, ...) \
namespace CONNECTOR { \
    auto onRequestSum = std::mem_fn(static_cast<RET (CLASS_REQUEST::*)(__VA_ARGS__)>(&CLASS_REQUEST::METHOD_REQUEST)); \
    auto onResponseSum = std::mem_fn(static_cast<void (CLASS_RESPONSE::*)(RET)>(&CLASS_RESPONSE::METHOD_RESPONSE)); \
    typedef std::tuple<__VA_ARGS__> Params; \
    class Request : public itc::_private::CallBinder \
    { \
    public: \
        Request(CLASS_REQUEST* contextRequest, Params params, CLASS_RESPONSE* contextResponse) \
            : CallBinder(THREAD_1, new itc::Request<CLASS_REQUEST, CLASS_RESPONSE, RET, ## __VA_ARGS__>(contextRequest, contextResponse, onRequestSum, onResponseSum, params)) \
            { \
                std::cout << itc::currentThreadName() << " ---REQUEST---> " << THREAD << ": " \
                << #CLASS_REQUEST << "::" << #METHOD_REQUEST << params << " RESP -> " << #CLASS_RESPONSE << "::" << #METHOD_RESPONSE << std::endl; \
            } \
    }; \
}

// #define DECLARE_REQUEST(CONNECTOR, CLASS_REQUEST, METHOD_REQUEST, CLASS_RESPONSE, METHOD_RESPONSE, TYPE_RESPONSE, ...) \
// namespace CONNECTOR { \
//     typedef itc::Request<CLASS_REQUEST, CLASS_RESPONSE, TYPE_RESPONSE, ##__VA_ARGS__> Request; \
//     auto METHOD_REQUEST = std::mem_fn(static_cast<TYPE_RESPONSE (CLASS_REQUEST::*)(__VA_ARGS__)>(&CLASS_REQUEST::METHOD_REQUEST)); \
//     auto METHOD_RESPONSE = std::mem_fn(static_cast<void (CLASS_RESPONSE::*)(TYPE_RESPONSE)>(&CLASS_RESPONSE::METHOD_RESPONSE)); \
// }

namespace itc {
    void createEventLoop(const std::string& threadName);
    bool invoke(const itc::_private::CallBinder& callBinder);
    bool invoke(const std::string& threadName, const _private::ICallable* call);
    Timer& createTimer(const std::string& threadName, const _private::ICallable* call, 
        std::chrono::milliseconds period, bool repeating);
    void deleteTimer(const std::string& threadName, const Timer& timer);
    int getLastTimerId();
    const std::string& currentThreadName();
}