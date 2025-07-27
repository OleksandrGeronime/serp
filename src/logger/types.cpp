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

#include "logger/types.hpp"

#include <ostream>
#include <type_traits>
#include <cstdint>

namespace serp
{

    std::ostream &operator<<(std::ostream &out, const eLogLevel &value)
    {
        switch (value)
        {
        case eLogLevel::fatal:
            out << "fatal";
            break;
        case eLogLevel::error:
            out << "error";
            break;
        case eLogLevel::warn:
            out << "warn ";
            break;
        case eLogLevel::info:
            out << "info ";
            break;
        case eLogLevel::debug:
            out << "debug";
            break;
        case eLogLevel::verbose:
            out << "verb ";
            break;
        case eLogLevel::off:
            out << "off  ";
            break;
        default:
            out << "eLogLevel::UNKNOWN_TYPE("
                << static_cast<std::uint32_t>(static_cast<std::underlying_type_t<eLogLevel>>(value))
                << ")";
            break;
        }
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const LoggingType &value)
    {
        switch (value)
        {
        case LoggingType::ENABLE:
            out << "ENABLE";
            break;
        case LoggingType::DISABLE:
            out << "DISABLE";
            break;
        case LoggingType::WITHOUT_PARAMS:
            out << "WITHOUT_PARAMS";
            break;
        default:
            out << "LoggingType::UNKNOWN_TYPE("
                << static_cast<std::uint32_t>(static_cast<std::underlying_type_t<LoggingType>>(value))
                << ")";
            break;
        }
        return out;
    }

} // namespace serp
