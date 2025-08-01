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

#include "App.hpp"
#include "Event.hpp"

namespace serp
{
    template <typename... Args>
    struct Async final
    {
        template <typename... CallArgs>
        Async(std::function<void(Args...)> fn, CallArgs &&...args)
        {
            App::invoke(
                App::threadName(),
                std::make_shared<Event<Args...>>(std::move(fn), std::make_tuple(std::forward<CallArgs>(args)...)));
        }

        template <typename... CallArgs>
        Async(const std::string &thread, std::function<void(Args...)> fn, CallArgs &&...args)
        {
            App::invoke(
                thread,
                std::make_shared<Event<Args...>>(std::move(fn), std::make_tuple(std::forward<CallArgs>(args)...)));
        }
    };

} // namespace  serp
