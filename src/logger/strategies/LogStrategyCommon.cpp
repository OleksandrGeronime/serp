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

#include "LogStrategyCommon.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <ostream>
#include <mutex>

namespace serp
{

    static auto g_start = std::chrono::steady_clock::now();
    static std::mutex g_timeMutex;

    void dumpDateTimePrefix(std::ostream &out)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        std::tm lt;
        {
            std::lock_guard<std::mutex> lock(g_timeMutex);
            lt = *std::localtime(&now_time);
        }

        const auto elapsedSeconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - g_start);

        out << std::put_time(&lt, "%Y.%m.%d %H.%M.%S") << " "
            << elapsedSeconds.count() << " ";
    }

} // namespace serp
