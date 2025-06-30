#pragma once

#include <cstdint>
#include <ostream>
#include <string>

namespace serp
{

    enum class eLogLevel : uint8_t
    {
        fatal,
        error,
        warn,
        info,
        debug,
        verbose,
        off
    };

    std::ostream &operator<<(std::ostream &out, const eLogLevel &value);

    enum class LoggingType : uint8_t
    {
        ENABLE,
        DISABLE,
        WITHOUT_PARAMS
    };

    std::ostream &operator<<(std::ostream &out, const LoggingType &value);

} // namespace serp
