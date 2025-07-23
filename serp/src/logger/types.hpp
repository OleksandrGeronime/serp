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
#include <ostream>
#include <string>

namespace serp
{

    enum class eLogLevel : uint8_t
    {
        fatal,
        error,
        warn,
        info,
        debug,
        verbose,
        off
    };

    std::ostream &operator<<(std::ostream &out, const eLogLevel &value);

    enum class LoggingType : uint8_t
    {
        ENABLE,
        DISABLE,
        WITHOUT_PARAMS
    };

    std::ostream &operator<<(std::ostream &out, const LoggingType &value);

} // namespace serp
