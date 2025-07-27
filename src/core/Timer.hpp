/*
 * Copyright 2025 Oleksandr Geronime
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 */

#pragma once

#include <chrono>
#include <functional>
#include <memory>

namespace serp
{

    class Timer
    {
    protected:
        Timer() = default;

    public:
        // Delete copy and move semantics
        Timer(const Timer &) = delete;
        Timer &operator=(const Timer &) = delete;
        Timer(Timer &&) = delete;
        Timer &operator=(Timer &&) = delete;

        virtual ~Timer() = default;

        /**
         * Creates a timer instance and registers it in the EventLoop.
         *
         * @param period Timer duration in milliseconds.
         * @param repeating Whether the timer repeats after expiration.
         * @param callback Function to execute on timer expiration.
         * @return Shared pointer to created Timer, or nullptr on failure.
         */
        static std::shared_ptr<Timer> create(
            const std::chrono::milliseconds &period,
            bool repeating,
            const std::function<void(std::shared_ptr<Timer>)> &callback);

        // Timer control methods
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void restart() = 0;

        // Timer state query methods
        virtual bool isStarted() const = 0;
        virtual bool isRepeating() const = 0;
        virtual int32_t getId() const = 0;
    };

    using TimerPtr = std::shared_ptr<Timer>;

} // namespace serp
