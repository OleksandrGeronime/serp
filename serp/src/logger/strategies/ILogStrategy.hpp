#pragma once

#include <cstdint>
#include <string>

#include "logger/types.hpp"

namespace serp
{

    class ILogStrategy
    {
    public:
        ILogStrategy() = default;
        virtual ~ILogStrategy() = default;

        virtual void init() = 0;

        virtual void log(const eLogLevel &level,
                         const std::string &tag,
                         const std::string &message) = 0;

        virtual void itcLog(const eLogLevel &level,
                            const std::string &senderThreadName,
                            const std::string &receiverThreadName,
                            const std::string &classAndMethodName,
                            const std::string &args,
                            bool isResponse) = 0;

        ILogStrategy(const ILogStrategy &) = delete;
        ILogStrategy &operator=(const ILogStrategy &) = delete;
    };

} // namespace serp
