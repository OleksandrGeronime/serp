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

#include <cstdint>
#include <string>

#include "logger/types.hpp"

namespace serp
{

    class ILogStrategy
    {
    public:
        ILogStrategy() = default;
        virtual ~ILogStrategy() = default;

        virtual void init() = 0;

        virtual void log(const eLogLevel &level,
                         const std::string &tag,
                         const std::string &message) = 0;

        virtual void itcLog(const eLogLevel &level,
                            const std::string &senderThreadName,
                            const std::string &receiverThreadName,
                            const std::string &classAndMethodName,
                            const std::string &args,
                            bool isResponse) = 0;

        ILogStrategy(const ILogStrategy &) = delete;
        ILogStrategy &operator=(const ILogStrategy &) = delete;
    };

} // namespace serp
