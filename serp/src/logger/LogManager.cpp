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
