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

<<<<<<< HEAD
#define DECLARE_CALL(CONNECTOR, CLASS, METHOD, ...) \
namespace CONNECTOR { \
    typedef itc::Call<CLASS, ##__VA_ARGS__> Call; \
    auto METHOD = std::mem_fn(static_cast<void (CLASS::*)(__VA_ARGS__)>(&CLASS::METHOD)); \
}

#define DECLARE_REQUEST(CONNECTOR, CLASS_REQUEST, METHOD_REQUEST, CLASS_RESPONSE, METHOD_RESPONSE, TYPE_RESPONSE, ...) \
namespace CONNECTOR { \
    typedef itc::Request<CLASS_REQUEST, CLASS_RESPONSE, TYPE_RESPONSE, ##__VA_ARGS__> Request; \
    auto METHOD_REQUEST = std::mem_fn(static_cast<TYPE_RESPONSE (CLASS_REQUEST::*)(__VA_ARGS__)>(&CLASS_REQUEST::METHOD_REQUEST)); \
    auto METHOD_RESPONSE = std::mem_fn(static_cast<void (CLASS_RESPONSE::*)(TYPE_RESPONSE)>(&CLASS_RESPONSE::METHOD_RESPONSE)); \
}

namespace itc {
    void createEventLoop(const std::string& threadName);
    bool invoke(const std::string& threadName, const _private::ICallable* call);
    Timer& createTimer(const std::string& threadName, const _private::ICallable* call, 
        std::chrono::milliseconds period, bool repeating);
    void deleteTimer(const std::string& threadName, const Timer& timer);
    int getLastTimerId();
    const std::string& currentThreadName();
=======
namespace itc {
    void createEventLoop(const std::string& threadName);
    bool invoke(const std::string& threadName, const _private::ICallable* call);
	Timer& createTimer(const std::string& threadName, const _private::ICallable* call, 
		std::chrono::milliseconds period, bool repeating);
    void deleteTimer(const std::string& threadName, const Timer& timer);
	int getLastTimerId();
	const std::string& currentThreadName();
>>>>>>> 520db933c96997436910b6a57e95c7dbcc8b23cc
}