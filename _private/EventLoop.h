/** @file EventLoop.h
 *  @brief EventLoop class declaration
 *
 *  Provides event loop functionality, wrap threads for processing async events
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include <thread>
#include <queue>
#include <list>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <iostream>

#include "../Call.h"
#include "Timer.h"

namespace itc {
namespace _private {

class Event;

class EventLoop 
{
public:
	EventLoop(const std::string& threadName);
	~EventLoop();

	bool createThread();
	void exitThread();

	std::thread::id getThreadId() const;
	const std::string& getThreadName() const;
	static std::thread::id getCurrentThreadId();
	inline int getLastTimerId() { return mLastTimerId; }
	
	void push(const ICallable* request);
	Timer& addTimer(const ICallable* call, std::chrono::milliseconds period, bool repeating);

private:
	EventLoop(const EventLoop&);
	EventLoop& operator=(const EventLoop&);

	void call();

	std::thread* mThread;
	std::queue<Event*> mEvents;
	std::list<Timer> mTimers;
	static int mNextTimerId;
	int mLastTimerId;
	std::mutex mMutex;
	std::condition_variable mCV;
	const std::string mThreadName;
};

}
}