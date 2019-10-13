/** @file EventLoop.cpp
 *  @brief EventLoop class implementation
 *
 *  Provides event loop functionality, wrap threads for processing async events
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#include "EventLoop.h"
//#include <iostream>
#include <string>
#include "Event.h"


namespace itc {
	namespace _private {

		int	EventLoop::mNextTimerId = 0;

		EventLoop::EventLoop(const std::string& threadName) : mThread(0), mThreadName(threadName)
		{
		}

		EventLoop::~EventLoop()
		{
			exitThread();
		}

		bool EventLoop::createThread()
		{
			if (!mThread)
				mThread = new std::thread(&EventLoop::call, this);	
			return true;
		}

		std::thread::id EventLoop::getThreadId() const
		{
			return mThread->get_id();
		}

		const std::string& EventLoop::getThreadName() const
		{
			return mThreadName;
		}

		std::thread::id EventLoop::getCurrentThreadId()
		{
			return std::this_thread::get_id();
		}

		void EventLoop::push(const ICallable* request)
		{
			Event* event = new Event(EventType::CALL, request);

			std::unique_lock<std::mutex> lk(mMutex);
			mEvents.push(event);
			mCV.notify_one();
		}

		Timer& EventLoop::addTimer(const ICallable* call, std::chrono::milliseconds period, bool repeating)
		{
			std::unique_lock<std::mutex> lock(mMutex);
			mTimers.emplace_back(call, period, repeating, mNextTimerId);
			++mNextTimerId;
			mCV.notify_one();
			return mTimers.back();
		}

		void EventLoop::exitThread()
		{
			if (!mThread)
				return;

			Event* event = new Event(EventType::SYSTEM, 0);
			{
				std::lock_guard<std::mutex> lock(mMutex);
				mEvents.push(event);
				mCV.notify_one();
			}

			mThread->join();
			delete mThread;
			mThread = 0;
		}

		void EventLoop::call()
		{   
			//std::cout << "0" << std::endl;
	
			while (1)
			{

				//std::cout << "1" << std::endl;

				const Event* event = nullptr;
				bool bDeleteCallable = true;
				{
					std::unique_lock<std::mutex> lock(mMutex);
					

					auto timeToNextTimer = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::duration::max());
					if (!mTimers.empty()) {
						using namespace std::chrono_literals;
						Timer& timer = mTimers.front();
						auto now = std::chrono::high_resolution_clock::now();
						auto dur = now - timer.getStartedTime();
						timeToNextTimer = timer.getPeriod() - std::chrono::duration_cast<std::chrono::milliseconds>(dur);
						if (timeToNextTimer <= 0ms) {
							event = timer.getEvent();
							mLastTimerId = timer.getId();
							if (timer.isRepeating()) {
								timer.start();
								bDeleteCallable = false;
							}
							else {
								mTimers.remove(timer);
							}
						}
					}

					if (event == nullptr) {
						//while (mEvents.empty())
							mCV.wait_for(lock, timeToNextTimer);

						if (mEvents.empty())
							continue;

						event = mEvents.front();
						mEvents.pop();
					}
				}

				switch (event->getType())
				{
				case EventType::CALL:
				{
					const ICallable* callable = event->getCallable();
					callable->call();
					break;
				}

				case EventType::SYSTEM:
				{
					std::unique_lock<std::mutex> lock(mMutex);
					Event* eventToDelete = nullptr;
					while (!mEvents.empty())
					{
						eventToDelete = mEvents.front();
						mEvents.pop();
						delete eventToDelete;
					}

					std::cout << "Exit thread on " << mThreadName << std::endl;
					return;
				}

				default:
					break;
				}

				if (bDeleteCallable) {
					delete event;
				}
			}
		}

	}
}