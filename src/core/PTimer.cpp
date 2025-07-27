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

#include "PTimer.hpp"

#include "App.hpp"
#include "EventLoop.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    PTimer::PTimer(const std::chrono::milliseconds &period,
                   const bool repeating,
                   const std::function<void(std::shared_ptr<Timer>)> action)
        : _action(action),
          _period(period),
          _repeating(repeating),
          _started(false),
          _startTime{} {}

    PTimer::~PTimer()
    {
        const auto EventLoop = App::instance().getLoopById(std::this_thread::get_id());
        if (EventLoop)
        {
            EventLoop->removeTimer(_id);
        }
        else
        {
            serp::logError() << "PTimer::~PTimer: Failed to get EventLoop for thread ID "
                             << std::this_thread::get_id();
        }
    }

    void PTimer::start()
    {
        if (!_started)
        {
            restart();
        }
        else
        {
            serp::logWarn() << "PTimer::start: Timer already started";
        }
    }

    void PTimer::stop()
    {
        _started = false;

        const auto EventLoop = App::instance().getLoopById(std::this_thread::get_id());
        if (EventLoop)
        {
            EventLoop->sortTimers();
        }
        else
        {
            serp::logError() << "PTimer::stop: Failed to get EventLoop for thread ID "
                             << std::this_thread::get_id();
        }
    }

    void PTimer::restart()
    {
        _startTime = std::chrono::steady_clock::now();
        _started = true;

        const auto EventLoop = App::instance().getLoopById(std::this_thread::get_id());
        if (EventLoop)
        {
            EventLoop->sortTimers();
        }
        else
        {
            serp::logError() << "PTimer::restart: Failed to get EventLoop for thread ID "
                             << std::this_thread::get_id();
        }
    }

} // namespace serp
