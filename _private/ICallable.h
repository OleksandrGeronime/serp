/** @file ICallable.h
 *  @brief Interface for callable objects
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

namespace itc {

	namespace _private {

		class ICallable {
		public:
			virtual ~ICallable() {};
			virtual void call() const = 0;
		};

	}
}
