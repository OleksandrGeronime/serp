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
