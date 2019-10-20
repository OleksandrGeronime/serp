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
#include <iostream>

#define DECLARE_CALL(CONNECTOR, THREAD, CLASS, METHOD, ...) \
namespace CONNECTOR { \
    auto METHOD = std::mem_fn(static_cast<void (CLASS::*)(__VA_ARGS__)>(&CLASS::METHOD)); \
	typedef std::tuple<__VA_ARGS__> Params; \
	class Call : public CallBinder \
	{ \
	public: \
		Call(CLASS* context, Params params) \
			: mpCallable(new itc::Call2<CLASS, __VA_ARGS__>(context, METHOD, params)) \
			, mThreadName(THREAD) \
		{ \
			std::cout << "INVOKE CALL " << #CLASS << std::endl; \
		} \
		const itc::_private::ICallable* getCallable() const override { return mpCallable; } \
		const std::string& getThreadName() const override { return mThreadName; } \
	private: \
		const itc::_private::ICallable* mpCallable; \
		const std::string mThreadName; \
	}; \
} 

#define DECLARE_REQUEST(CONNECTOR, CLASS_REQUEST, METHOD_REQUEST, CLASS_RESPONSE, METHOD_RESPONSE, TYPE_RESPONSE, ...) \
namespace CONNECTOR { \
    typedef itc::Request<CLASS_REQUEST, CLASS_RESPONSE, TYPE_RESPONSE, ##__VA_ARGS__> Request; \
    auto METHOD_REQUEST = std::mem_fn(static_cast<TYPE_RESPONSE (CLASS_REQUEST::*)(__VA_ARGS__)>(&CLASS_REQUEST::METHOD_REQUEST)); \
    auto METHOD_RESPONSE = std::mem_fn(static_cast<void (CLASS_RESPONSE::*)(TYPE_RESPONSE)>(&CLASS_RESPONSE::METHOD_RESPONSE)); \
}


class CallBinder {
public:
	virtual ~CallBinder() {};
	virtual const itc::_private::ICallable* getCallable() const = 0;
	virtual const std::string& getThreadName() const = 0;
};

namespace itc {
    void createEventLoop(const std::string& threadName);
	bool invoke(const CallBinder& callBinder);
    bool invoke(const std::string& threadName, const _private::ICallable* call);
    Timer& createTimer(const std::string& threadName, const _private::ICallable* call, 
        std::chrono::milliseconds period, bool repeating);
    void deleteTimer(const std::string& threadName, const Timer& timer);
    int getLastTimerId();
    const std::string& currentThreadName();
}