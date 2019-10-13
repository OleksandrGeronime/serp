/** @file Event.h
 *  @brief Event class declaration
 *
 *  Provides event wich contains callable object pointer and could be pushed in event loop
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include "ICallable.h"

namespace itc {
	namespace _private {

		enum EventType {
			SYSTEM,
			CALL,
			TIMER,
		};

		enum EventPriority {
			LOWEST,
			LOW,
			MEDIUM,
			HIGH,
			HIGHEST
		};

		class Event
		{
		public:
			Event(EventType type, const itc::_private::ICallable* callable)
				: mType(type)
				, mPriority(EventPriority::MEDIUM)
				, mpCallable(callable)
			{}

			Event(EventType type, EventPriority priority, const itc::_private::ICallable* callable)
				: mType(type)
				, mPriority(priority)
				, mpCallable(callable)
			{}

			~Event()
			{
				delete mpCallable;
			}

			inline EventType getType() const {
				return mType;
			}

			inline EventPriority getPriority() const {
				return mPriority;
			}

			inline const ICallable* getCallable() const {
				return mpCallable;
			}

		private:
			const EventType mType;
			const EventPriority mPriority;
			const ICallable* mpCallable;
		};
	}
}