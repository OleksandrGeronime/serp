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

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

namespace serp
{

    /**
     * @brief Monitors registered threads for timeout violations using watchdog timers.
     */
    class Watchdog final
    {
    public:
        Watchdog() = default;
        virtual ~Watchdog() = default;

        void start();
        void stop();

        void registerThread(const std::thread::id &threadId,
                            const std::chrono::milliseconds &timeout);
        void unregisterThread(const std::thread::id &threadId);
        void enterEvent(const std::thread::id &threadId);
        void exitEvent(const std::thread::id &threadId);

    private:
        Watchdog(const Watchdog &) = delete;
        Watchdog &operator=(const Watchdog &) = delete;
        Watchdog(Watchdog &&) = delete;
        Watchdog &operator=(Watchdog &&) = delete;

        std::thread::id getNextTimeoutThread();

        std::mutex _mutex;
        std::condition_variable _cv;
        std::atomic_bool _stopped{false};

        std::map<std::thread::id, std::chrono::milliseconds> _timeouts;
        std::map<std::thread::id, std::chrono::steady_clock::time_point> _startTimes;
    };

} // namespace serp
