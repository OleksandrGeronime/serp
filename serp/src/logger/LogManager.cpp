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

#include "LogManager.hpp"
#include "serp.hpp"

namespace serp
{

    LogManager &LogManager::getInstance()
    {
        static LogManager instance;
        return instance;
    }

    LogManager::~LogManager() = default;

    void LogManager::log(const std::string &threadId,
                         const eLogLevel &level,
                         const std::string &tag,
                         const std::string &message)
    {
        (void)threadId;

        const std::lock_guard<std::mutex> lock(_mutex);
        for (const auto &strategy : _strategies)
        {
            strategy->log(level, tag, message);
        }
    }

    void LogManager::registerStrategy(const std::shared_ptr<ILogStrategy> &strategy)
    {
        const std::lock_guard<std::mutex> lock(_mutex);
        strategy->init();
        _strategies.push_back(strategy);
    }

    void LogManager::clearStrategies()
    {
        const std::lock_guard<std::mutex> lock(_mutex);
        _strategies.clear();
    }

    const std::string &LogManager::threadName()
    {
        return App::threadName();
    }

    void LogManager::logMethod(const std::string &methodName)
    {
        auto logStream = std::make_unique<LogWriter>(
            threadName(), "", eLogLevel::info);
        *logStream << methodName << "()";
    }

    void LogManager::logMethod(const LogTag &tag,
                               const std::string &methodName)
    {
        auto logStream = std::make_unique<LogWriter>(
            threadName(), tag.getTag(), eLogLevel::info);
        *logStream << methodName << "()";
    }

} // namespace serp
