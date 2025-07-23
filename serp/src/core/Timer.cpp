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

#include "Timer.hpp"

#include "App.hpp"
#include "EventLoop.hpp"
#include "PTimer.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    std::shared_ptr<Timer> Timer::create(
        const std::chrono::milliseconds &period,
        bool repeating,
        const std::function<void(std::shared_ptr<Timer>)> &callback)
    {
        auto timer = std::make_shared<PTimer>(period, repeating, callback);
        if (!timer)
        {
            serp::logError() << "Timer::create: failed to allocate PTimer instance";
            return nullptr;
        }

        const auto ep = App::instance().getLoopById(std::this_thread::get_id());
        if (!ep)
        {
            serp::logError() << "Timer::create: no EventLoop found for thread " << std::this_thread::get_id();
            return nullptr;
        }

        const auto id = ep->addTimer(timer);
        timer->setId(id);

        if (id == 0)
        {
            serp::logError() << "Timer::create: failed to register timer in EventLoop";
            return nullptr;
        }

        return timer;
    }

} // namespace serp
