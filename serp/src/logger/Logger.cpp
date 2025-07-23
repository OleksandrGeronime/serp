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

#include "logger/Logger.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace
{

    void logImpl(const std::function<std::unique_ptr<serp::LogWriter>()> &logger, const char *fmt, va_list args)
    {
        if (!fmt)
            return;

        va_list argsCopy;
        va_copy(argsCopy, args);
        const int size = std::vsnprintf(nullptr, 0, fmt, argsCopy);
        va_end(argsCopy);

        if (size < 0)
            return;

        char *buffer = static_cast<char *>(std::malloc(size + 1));
        if (!buffer)
            return;

        std::vsnprintf(buffer, size + 1, fmt, args);

        logger() << buffer;

        std::free(buffer);
    }

} // namespace

extern "C"
{

    void logVerbose(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        logImpl([]
                { return serp::logVerbose(); }, fmt, args);
        va_end(args);
    }

    void logDebug(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        logImpl([]
                { return serp::logDebug(); }, fmt, args);
        va_end(args);
    }

    void logInfo(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        logImpl([]
                { return serp::logInfo(); }, fmt, args);
        va_end(args);
    }

    void logWarn(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        logImpl([]
                { return serp::logWarn(); }, fmt, args);
        va_end(args);
    }

    void logError(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        logImpl([]
                { return serp::logError(); }, fmt, args);
        va_end(args);
    }
}
