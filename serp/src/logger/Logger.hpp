#pragma once

#include <cstdint>
#include <ostream>
#include <sstream>
#include <string>
#include <tuple>

namespace serp
{

    // Defines log severity levels.
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

    // Controls whether to log request/response parameters.
    enum class LoggingType : uint8_t
    {
        ENABLE,
        DISABLE,
        WITHOUT_PARAMS
    };

    // Dummy logging streams to suppress output during stubbing.
    inline std::ostream &logVerbose()
    {
        static std::ostringstream _dummy_stream;
        return _dummy_stream;
    }

    inline std::ostream &logDebug()
    {
        static std::ostringstream _dummy_stream;
        return _dummy_stream;
    }

    inline std::ostream &logInfo()
    {
        static std::ostringstream _dummy_stream;
        return _dummy_stream;
    }

    inline std::ostream &logWarn()
    {
        static std::ostringstream _dummy_stream;
        return _dummy_stream;
    }

    inline std::ostream &logError()
    {
        static std::ostringstream _dummy_stream;
        return _dummy_stream;
    }

    // Logs entry into a method without parameters.
    inline void logMethod(const std::string &method_name)
    {
        [[maybe_unused]] const auto &_name = method_name;
    }

    // Logs entry into a method with parameters (variadic).
    template <typename Arg, typename... Args>
    void logMethod(const std::string &method_name, Arg &&arg, Args &&...args)
    {
        [[maybe_unused]] const auto &_name = method_name;
        [[maybe_unused]] const auto &_args = std::forward_as_tuple(std::forward<Arg>(arg), std::forward<Args>(args)...);
    }

    // Logs entry into a method where method name is passed as stringstream.
    template <typename Arg, typename... Args>
    void logMethod(const std::stringstream &method_name, Arg &&arg, Args &&...args)
    {
        [[maybe_unused]] const auto &_name = method_name;
        [[maybe_unused]] const auto &_args = std::forward_as_tuple(std::forward<Arg>(arg), std::forward<Args>(args)...);
    }

    // Generic logger for requests and responses (stub).
    template <class... Args>
    void itcLog(const eLogLevel &level,
                const std::string &sender_thread_name,
                const std::string &receiver_thread_name,
                const std::string &class_and_method_name,
                const std::tuple<Args...> &args,
                bool is_response = false)
    {
        [[maybe_unused]] const auto &_lvl = level;
        [[maybe_unused]] const auto &_from = sender_thread_name;
        [[maybe_unused]] const auto &_to = receiver_thread_name;
        [[maybe_unused]] const auto &_method = class_and_method_name;
        [[maybe_unused]] const auto &_args = args;
        [[maybe_unused]] const auto _resp = is_response;
    }

} // namespace serp
