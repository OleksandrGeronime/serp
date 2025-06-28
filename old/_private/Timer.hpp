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
#include <memory>

namespace itc {

	namespace _private {
		class Event;
		class ICallable;
	}

	class ITimer
	{
	public:
		virtual ~ITimer() = default;

		virtual void call() = 0;

		virtual void start() = 0;
		virtual void stop() = 0;

		virtual std::chrono::milliseconds getPeriod() const = 0;
		virtual std::chrono::milliseconds timeLast() const = 0;
		virtual bool isStarted() const = 0;
		virtual bool isRepeating() const = 0;
		virtual std::chrono::system_clock::time_point getStartedTime() const = 0;
		virtual int getId() const = 0;
	};

	class Timer: public ITimer
	{
	public:
		Timer(std::shared_ptr<_private::ICallable> callable, std::chrono::milliseconds period, bool repeating, int timerId);
        virtual ~Timer();

        Timer(const Timer&) = delete;
        Timer& operator=(const Timer&) = delete;
        Timer(Timer&&) = delete;
        Timer& operator=(Timer&&) = delete;

		void call() override;
		void start() override;
		void stop() override;

		std::chrono::milliseconds getPeriod() const override { return mPeriod; }
		std::chrono::milliseconds timeLast() const override { return mPeriod; }
		bool isStarted() const override { return mbStarted; };
		bool isRepeating() const override { return mbRepeating; }
		std::chrono::system_clock::time_point getStartedTime() const override { return mStartedTime; }
		int getId() const override { return mTimerId; }
		std::shared_ptr<_private::Event> getEvent() const { return mpEvent; }

		bool operator==(const itc::Timer& timer) {
			return this->mTimerId == timer.mTimerId;
		}

	private:
		std::shared_ptr<_private::Event> mpEvent;
		std::chrono::milliseconds mPeriod;
		bool mbRepeating;
		bool mbStarted;
		std::chrono::system_clock::time_point mStartedTime;
		int mTimerId;
	};
}