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

#include <memory>
#include <string>
#include <tuple>
#include <sstream>
#include <utility>

#include "Loggable.hpp"
#include "LogManager.hpp"

namespace serp
{

    inline std::unique_ptr<LogWriter> logVerbose()
    {
        return std::make_unique<LogWriter>(
            LogManager::getInstance().threadName(), "", eLogLevel::verbose);
    }

    inline std::unique_ptr<LogWriter> logDebug()
    {
        return std::make_unique<LogWriter>(
            LogManager::getInstance().threadName(), "", eLogLevel::debug);
    }

    inline std::unique_ptr<LogWriter> logInfo()
    {
        return std::make_unique<LogWriter>(
            LogManager::getInstance().threadName(), "", eLogLevel::info);
    }

    inline std::unique_ptr<LogWriter> logWarn()
    {
        return std::make_unique<LogWriter>(
            LogManager::getInstance().threadName(), "", eLogLevel::warn);
    }

    inline std::unique_ptr<LogWriter> logError()
    {
        return std::make_unique<LogWriter>(
            LogManager::getInstance().threadName(), "", eLogLevel::error);
    }

    template <typename... Args>
    inline void itcLog(eLogLevel level,
                       const std::string &senderThreadName,
                       const std::string &receiverThreadName,
                       const std::string &classAndMethodName,
                       const std::tuple<Args...> &args,
                       bool isResponse = false)
    {
        LogManager::getInstance().itcLog(
            level, senderThreadName, receiverThreadName, classAndMethodName, args, isResponse);
    }

    inline void logMethod(const std::string &methodName)
    {
        LogManager::logMethod(methodName);
    }

    template <typename Arg, typename... Args>
    inline void logMethod(const std::string &methodName, Arg &&arg, Args &&...args)
    {
        LogManager::logMethod(methodName, std::forward<Arg>(arg), std::forward<Args>(args)...);
    }

    template <typename Arg, typename... Args>
    inline void logMethod(const std::stringstream &methodName, Arg &&arg, Args &&...args)
    {
        LogManager::logMethod(methodName.str(), std::forward<Arg>(arg), std::forward<Args>(args)...);
    }

    namespace logger
    {

        template <typename T>
        inline void addStrategy()
        {
            LogManager::getInstance().registerStrategy(std::make_shared<T>());
        }

        template <typename T, typename... Args>
        inline void addStrategy(Args &&...args)
        {
            LogManager::getInstance().registerStrategy(std::make_shared<T>(std::forward<Args>(args)...));
        }

        inline void clearStrategies()
        {
            LogManager::getInstance().clearStrategies();
        }

    } // namespace logger

} // namespace serp

extern "C" void logVerbose(const char *fmt, ...);
extern "C" void logDebug(const char *fmt, ...);
extern "C" void logInfo(const char *fmt, ...);
extern "C" void logWarn(const char *fmt, ...);
extern "C" void logError(const char *fmt, ...);
