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

#include "Timer.hpp"

namespace serp
{
    class Invokable;

    /**
     * @brief Implementation of a software timer with start/stop logic and expiration tracking.
     */
    class PTimer final : public Timer, public std::enable_shared_from_this<PTimer>
    {
    public:
        /**
         * @brief Construct a new PTimer object.
         *
         * @param period Duration before timer expiration.
         * @param repeating Whether the timer should repeat.
         * @param action Callback executed on expiration.
         */
        PTimer(const std::chrono::milliseconds &period,
               bool repeating,
               std::function<void(std::shared_ptr<Timer>)> action);

        ~PTimer() override;

        // Overrides from Timer interface
        void start() override;
        void stop() override;
        void restart() override;

        bool isStarted() const override { return _started; }
        bool isRepeating() const override { return _repeating; }
        int32_t getId() const override { return _id; }

        /**
         * @brief Refreshes the start time to now.
         * @return New start time.
         */
        std::chrono::steady_clock::time_point updateStartedTime()
        {
            _startTime = std::chrono::steady_clock::now();
            return _startTime;
        }

        std::chrono::milliseconds getPeriod() const { return _period; }
        std::chrono::milliseconds timeLast() const { return _period; }

        std::chrono::steady_clock::time_point getStartedTime() const
        {
            return _startTime;
        }

        std::chrono::steady_clock::time_point getExpirationTime() const
        {
            return _startTime + _period;
        }

        void setId(int32_t id) { _id = id; }

        std::function<void(std::shared_ptr<Timer>)> getAction() const
        {
            return _action;
        }

        bool operator<(const PTimer &other) const
        {
            return getExpirationTime() < other.getExpirationTime();
        }

        bool operator==(const PTimer &other) const
        {
            return _id == other._id;
        }

    private:
        std::function<void(std::shared_ptr<Timer>)> _action;
        std::chrono::milliseconds _period;
        std::chrono::steady_clock::time_point _startTime;

        bool _repeating = false;
        bool _started = false;
        int32_t _id = 0;
    };

} // namespace serp
