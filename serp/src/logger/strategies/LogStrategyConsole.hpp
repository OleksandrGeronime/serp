#pragma once

#include <mutex>
#include <string>

#include "ILogStrategy.hpp"

namespace serp
{

    class LogStrategyConsole : public ILogStrategy
    {
    public:
        LogStrategyConsole() noexcept;
        ~LogStrategyConsole() override = default;

        void init() override;

        void log(const eLogLevel &level, const std::string &prefix,
                 const std::string &message) override;

        void itcLog(const eLogLevel &level, const std::string &senderThreadName,
                    const std::string &receiverThreadName, const std::string &classAndMethodName,
                    const std::string &args, bool isResponse) override;

    private:
        std::mutex _mutex;
    };

} // namespace serp
