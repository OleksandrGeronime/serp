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

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "logger/types.hpp"

namespace serp
{

    class LogWriter
    {
    public:
        LogWriter(const std::string &threadId, const std::string &tag, eLogLevel logLevel);
        LogWriter(LogWriter &&other) noexcept;
        LogWriter(const LogWriter &) = delete;
        LogWriter &operator=(const LogWriter &) = delete;
        LogWriter &operator=(LogWriter &&other) noexcept;
        ~LogWriter() noexcept;

        template <typename ValueT>
        std::ostream &operator<<(const ValueT &value)
        {
            _baseStream << std::boolalpha << value;
            return _baseStream;
        }

    private:
        std::ostringstream _baseStream;
        std::string _threadName;
        std::string _tag;
        eLogLevel _level;
    };

    template <typename ValueT>
    std::unique_ptr<LogWriter> operator<<(std::unique_ptr<LogWriter> stream, const ValueT &value)
    {
        if (stream)
        {
            *stream << value;
        }
        return stream;
    }

} // namespace serp
