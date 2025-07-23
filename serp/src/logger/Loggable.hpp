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

#include "LogManager.hpp"
#include "LogTag.hpp"

namespace serp
{

    class Loggable
    {
    public:
        Loggable() = default;
        virtual ~Loggable() = default;

        // Each subclass must define its log tag
        virtual std::string tag() const = 0;

        void link(const std::weak_ptr<Loggable> &parent)
        {
            _parent = parent;
        }

        std::unique_ptr<LogWriter> logVerbose() const
        {
            return std::make_unique<LogWriter>(
                LogManager::getInstance().threadName(),
                getFullPrefix(),
                eLogLevel::verbose);
        }

        std::unique_ptr<LogWriter> logDebug() const
        {
            return std::make_unique<LogWriter>(
                LogManager::getInstance().threadName(),
                getFullPrefix(),
                eLogLevel::debug);
        }

        std::unique_ptr<LogWriter> logInfo() const
        {
            return std::make_unique<LogWriter>(
                LogManager::getInstance().threadName(),
                getFullPrefix(),
                eLogLevel::info);
        }

        std::unique_ptr<LogWriter> logWarn() const
        {
            return std::make_unique<LogWriter>(
                LogManager::getInstance().threadName(),
                getFullPrefix(),
                eLogLevel::warn);
        }

        std::unique_ptr<LogWriter> logError() const
        {
            return std::make_unique<LogWriter>(
                LogManager::getInstance().threadName(),
                getFullPrefix(),
                eLogLevel::error);
        }

        template <typename... Args>
        void logMethod(const std::string &methodName, Args &&...args) const
        {
            LogManager::logMethod(
                LogTag(getFullPrefix()),
                methodName,
                std::forward<Args>(args)...);
        }

    private:
        std::string getFullPrefix() const
        {
            if (auto parent = _parent.lock())
            {
                return parent->getFullPrefix() + tag() + "::";
            }
            return tag() + "::";
        }

        std::weak_ptr<Loggable> _parent;
    };

} // namespace serp
