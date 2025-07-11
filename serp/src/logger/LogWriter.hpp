#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "logger/types.hpp"

namespace serp
{

    class LogWriter
    {
    public:
        LogWriter(const std::string &threadId, const std::string &tag, eLogLevel logLevel);
        LogWriter(LogWriter &&other) noexcept;
        LogWriter(const LogWriter &) = delete;
        LogWriter &operator=(const LogWriter &) = delete;
        LogWriter &operator=(LogWriter &&other) noexcept;
        ~LogWriter() noexcept;

        template <typename ValueT>
        std::ostream &operator<<(const ValueT &value)
        {
            _baseStream << std::boolalpha << value;
            return _baseStream;
        }

    private:
        std::ostringstream _baseStream;
        std::string _threadName;
        std::string _tag;
        eLogLevel _level;
    };

    template <typename ValueT>
    std::unique_ptr<LogWriter> operator<<(std::unique_ptr<LogWriter> stream, const ValueT &value)
    {
        if (stream)
        {
            *stream << value;
        }
        return stream;
    }

} // namespace serp
