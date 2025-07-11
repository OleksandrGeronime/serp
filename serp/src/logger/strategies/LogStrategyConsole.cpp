#include "LogStrategyConsole.hpp"

#include <iostream>

#include "serp.hpp"
#include "LogStrategyCommon.hpp"

namespace serp
{

    // ANSI color codes
    namespace
    {
        constexpr const char *RESET = "\033[0m";
        constexpr const char *RED = "\033[31m";
        constexpr const char *BOLD_RED = "\033[1m\033[31m";
        constexpr const char *YELLOW = "\033[33m";
        constexpr const char *GREEN = "\033[32m";
        constexpr const char *WHITE = "\033[37m";
        constexpr const char *DEFAULT_COL = "\033[39m";
    }

    LogStrategyConsole::LogStrategyConsole() noexcept = default;

    void LogStrategyConsole::init()
    {
        std::cout.setf(std::ios::fixed, std::ios::floatfield);
    }

    void LogStrategyConsole::log(const eLogLevel &level, const std::string &tag,
                                 const std::string &message)
    {
        const std::unique_lock lock(_mutex);

        dumpDateTimePrefix(std::cout);

        // Select color by log level
        const char *color = nullptr;
        switch (level)
        {
        case eLogLevel::fatal:
            color = BOLD_RED;
            break;
        case eLogLevel::error:
            color = RED;
            break;
        case eLogLevel::warn:
            color = YELLOW;
            break;
        case eLogLevel::info:
            color = WHITE;
            break;
        case eLogLevel::debug:
        case eLogLevel::verbose:
        default:
            color = DEFAULT_COL;
            break;
        }

        std::cout << color << level << " [" << serp::App::threadName() << "] "
                  << tag << message << RESET << std::endl;
    }

    void LogStrategyConsole::itcLog(const eLogLevel &level,
                                    const std::string &senderThreadName,
                                    const std::string &receiverThreadName,
                                    const std::string &classAndMethodName,
                                    const std::string &args, bool isResponse)
    {
        const std::unique_lock lock(_mutex);

        dumpDateTimePrefix(std::cout);

        std::cout << GREEN << level << " [" << senderThreadName
                  << (isResponse ? "<-" : "->") << receiverThreadName << "] "
                  << classAndMethodName << "(" << args << ")" << RESET << std::endl;
    }

} // namespace serp
