#pragma once

#include <memory>
#include <mutex>
#include <sstream>
#include <vector>

#include "Dumpers.hpp"
#include "logger/strategies/ILogStrategy.hpp"
#include "LogTag.hpp"
#include "LogWriter.hpp"

// Logging levels for compile-time filtering
#define LOG_LEVEL_OFF 0
#define LOG_LEVEL_FATAL 1
#define LOG_LEVEL_ERROR 2
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_INFO 4
#define LOG_LEVEL_DEBUG 5
#define LOG_LEVEL_VERBOSE 6

namespace serp
{

    class LogManager
    {
    public:
        ~LogManager();
        static LogManager &getInstance();

        void log(const std::string &threadId,
                 const eLogLevel &level,
                 const std::string &tag,
                 const std::string &message);

        void registerStrategy(const std::shared_ptr<ILogStrategy> &strategy);
        void clearStrategies();

        static const std::string &threadName();

        static void logMethod(const std::string &methodName);
        static void logMethod(const LogTag &tag, const std::string &methodName);

        template <typename Arg, typename... Args>
        static void logMethod(const std::string &methodName, Arg &&arg, Args &&...args)
        {
            auto stream = std::make_unique<LogWriter>(
                LogManager::getInstance().threadName(),
                "", eLogLevel::info);

            *stream << methodName << "(" << std::forward<Arg>(arg);
            (..., (*stream << ", " << std::forward<Args>(args)));
            *stream << ")";
        }

        template <typename Arg, typename... Args>
        static void logMethod(const LogTag &tag, const std::string &methodName, Arg &&arg, Args &&...args)
        {
            auto stream = std::make_unique<LogWriter>(
                LogManager::getInstance().threadName(),
                tag.getTag(), eLogLevel::info);

            *stream << methodName << "(" << std::forward<Arg>(arg);
            (..., (*stream << ", " << std::forward<Args>(args)));
            *stream << ")";
        }

        template <typename... Args>
        void itcLog(const eLogLevel &level,
                    const std::string &senderThread,
                    const std::string &receiverThread,
                    const std::string &methodName,
                    const std::tuple<Args...> &args,
                    bool isResponse = false)
        {
            do
            {
#if defined(LOG_LEVEL)
#if LOG_LEVEL == LOG_LEVEL_OFF
                break;
#elif LOG_LEVEL < LOG_LEVEL_VERBOSE
                if (level > static_cast<eLogLevel>(LOG_LEVEL))
                    break;
#endif
#else
                if (level > eLogLevel::info)
                    break;
#endif
                std::stringstream ss;
                printTuple(ss, args);

                const std::lock_guard<std::mutex> lock(_mutex);
                for (auto &strategy : _strategies)
                {
                    strategy->itcLog(level, senderThread, receiverThread, methodName, ss.str(), isResponse);
                }
            } while (false);
        }

    private:
        LogManager() = default;

        std::vector<std::shared_ptr<ILogStrategy>> _strategies;
        std::mutex _mutex;
    };

} // namespace serp
