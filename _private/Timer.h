/** @file Timer.h
 *  @brief Timer class declaration
 *
 *  Provides timers functionality
 *
 *  @date 2019
 *  @author Alexander Geronime 
 */

#pragma once

#include <string>
#include <chrono>
#include "ICallable.h"
#include "Event.h"

namespace itc {
	namespace _private {

		class ITimer
		{
		public:
			virtual ~ITimer() {}

			virtual void call() = 0;

			virtual void start() = 0;
			virtual void stop() = 0;

			virtual std::chrono::milliseconds getPeriod() const = 0;
			virtual std::chrono::milliseconds timeLast() const = 0;
			virtual bool isRepeating() const = 0;
			virtual std::chrono::steady_clock::time_point getStartedTime() const = 0;
			virtual int getId() const = 0;
		};

		class Timer: public ITimer
		{
		public:
			Timer(const _private::ICallable* callable, std::chrono::milliseconds period, bool repeating, int timerId)
				: mpEvent(new Event(EventType::CALL, callable))
				, mPeriod(period)
				, mbRepeating(repeating)
				, mbStarted(false)
				, mStartedTime()
				, mTimerId(timerId)
			{}

			void call() override {
				mpEvent->getCallable()->call();
			}

			void start() override {
				mStartedTime = std::chrono::high_resolution_clock::now();
				mbStarted = true;
			};
			void stop() override { mbStarted = false; };

			std::chrono::milliseconds getPeriod() const override { return mPeriod; }
			std::chrono::milliseconds timeLast() const override { return mPeriod; }
			bool isRepeating() const override { return mbRepeating; }
			std::chrono::steady_clock::time_point getStartedTime() const override { return mStartedTime; }
			int getId() const override { return mTimerId; }
			const Event* getEvent() const { return mpEvent; }

			bool operator==(const itc::_private::Timer& timer) {
				return this->mTimerId == timer.mTimerId;
			}

		private:
			const Event* mpEvent;
			std::chrono::milliseconds mPeriod;
			bool mbRepeating;
			bool mbStarted;
			std::chrono::steady_clock::time_point mStartedTime;
			int mTimerId;
		};
	}
}