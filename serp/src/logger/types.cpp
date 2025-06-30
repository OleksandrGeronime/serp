#include "logger/types.hpp"

#include <ostream>
#include <type_traits>
#include <cstdint>

namespace serp
{

    std::ostream &operator<<(std::ostream &out, const eLogLevel &value)
    {
        switch (value)
        {
        case eLogLevel::fatal:
            out << "fatal";
            break;
        case eLogLevel::error:
            out << "error";
            break;
        case eLogLevel::warn:
            out << "warn ";
            break;
        case eLogLevel::info:
            out << "info ";
            break;
        case eLogLevel::debug:
            out << "debug";
            break;
        case eLogLevel::verbose:
            out << "verb ";
            break;
        case eLogLevel::off:
            out << "off  ";
            break;
        default:
            out << "eLogLevel::UNKNOWN_TYPE("
                << static_cast<std::uint32_t>(static_cast<std::underlying_type_t<eLogLevel>>(value))
                << ")";
            break;
        }
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const LoggingType &value)
    {
        switch (value)
        {
        case LoggingType::ENABLE:
            out << "ENABLE";
            break;
        case LoggingType::DISABLE:
            out << "DISABLE";
            break;
        case LoggingType::WITHOUT_PARAMS:
            out << "WITHOUT_PARAMS";
            break;
        default:
            out << "LoggingType::UNKNOWN_TYPE("
                << static_cast<std::uint32_t>(static_cast<std::underlying_type_t<LoggingType>>(value))
                << ")";
            break;
        }
        return out;
    }

} // namespace serp
