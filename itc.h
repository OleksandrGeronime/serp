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

namespace itc {
    void createEventLoop(const std::string& threadName);
    bool invoke(const std::string& threadName, const _private::ICallable* call);
	Timer& createTimer(const std::string& threadName, const _private::ICallable* call, 
		std::chrono::milliseconds period, bool repeating);
    void deleteTimer(const std::string& threadName, const Timer& timer);
	int getLastTimerId();
	const std::string& currentThreadName();
}