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

#include "Timer.hpp"
#include "Async.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    template <typename... Args>
    class AsyncWithDelay final
    {
    public:
        template <typename... CallArgs>
        AsyncWithDelay(const std::chrono::milliseconds &delay,
                       std::function<void(Args...)> fn, CallArgs &&...args)
            : AsyncWithDelay(App::threadName(), delay, std::move(fn), std::forward<CallArgs>(args)...)
        {
        }

        template <typename... CallArgs>
        AsyncWithDelay(const std::string &thread,
                       const std::chrono::milliseconds &delay,
                       std::function<void(Args...)> fn, CallArgs &&...args)
        {
            Async<Args...>(
                thread,
                [this, delay, fn = std::move(fn)](CallArgs &&...forwarded)
                {
                    const bool oneShot = false;

                    _timer = Timer::create(delay, oneShot,
                                           [this,
                                            fn = std::move(fn),
                                            argsTuple = std::make_tuple(std::forward<CallArgs>(forwarded)...)](const std::shared_ptr<Timer> &) mutable
                                           {
                                               std::apply(fn, std::move(argsTuple));
                                               _timer.reset();
                                           });

                    if (_timer)
                    {
                        _timer->start();
                    }
                    else
                    {
                        serp::logError() << "AsyncWithDelay: Failed to create timer";
                    }
                },
                std::forward<CallArgs>(args)...);
        }

    private:
        std::shared_ptr<Timer> _timer;
    };

} // namespace serp
