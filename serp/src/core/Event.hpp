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

#include "Invoker.hpp"
#include "Invokable.hpp"

namespace serp
{

    template <typename... Args>
    class Event final : public Invokable
    {
    public:
        explicit Event(const std::function<void(Args...)> &func,
                       const std::tuple<Args...> &params = std::tuple<>())
            : _func(func), _params(params) {}

        void invoke() const override
        {
            invokeAsyncCallFunc(
                _func, typename make_indexes<Args...>::type(), std::move(_params));
        }

    private:
        const std::function<void(Args...)> _func;
        mutable std::tuple<Args...> _params;
    };

} // namespace serp
