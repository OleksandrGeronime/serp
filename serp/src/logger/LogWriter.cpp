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

#include "LogWriter.hpp"
#include "LogManager.hpp"

namespace serp
{

    LogWriter::LogWriter(const std::string &threadId, const std::string &tag, const eLogLevel logLevel)
        : _threadName(threadId), _tag(tag), _level(logLevel) {}

    LogWriter::LogWriter(LogWriter &&other) noexcept
        : _baseStream(std::move(other._baseStream)),
          _threadName(std::move(other._threadName)),
          _tag(std::move(other._tag)),
          _level(std::exchange(other._level, serp::eLogLevel::error)) {}

    LogWriter::~LogWriter() noexcept
    {
        do
        {
#if defined(LOG_LEVEL)
#if LOG_LEVEL == LOG_LEVEL_OFF
            break;
#elif LOG_LEVEL == LOG_LEVEL_FATAL
            if (eLogLevel::fatal < _level)
                break;
#elif LOG_LEVEL == LOG_LEVEL_ERROR
            if (eLogLevel::error < _level)
                break;
#elif LOG_LEVEL == LOG_LEVEL_WARN
            if (eLogLevel::warn < _level)
                break;
#elif LOG_LEVEL == LOG_LEVEL_INFO
            if (eLogLevel::info < _level)
                break;
#elif LOG_LEVEL == LOG_LEVEL_DEBUG
            if (eLogLevel::debug < _level)
                break;
#elif LOG_LEVEL == LOG_LEVEL_VERBOSE
            if (eLogLevel::verbose < _level)
                break;
#else
            if (eLogLevel::info < _level)
                break;
#endif
#else
            if (eLogLevel::info < _level)
                break;
#endif

            LogManager::getInstance().log(_threadName, _level, _tag, _baseStream.str());
        } while (false);
    }

    LogWriter &LogWriter::operator=(LogWriter &&other) noexcept
    {
        _baseStream.swap(other._baseStream);
        std::swap(_threadName, other._threadName);
        std::swap(_tag, other._tag);
        std::swap(_level, other._level);
        return *this;
    }

} // namespace serp
