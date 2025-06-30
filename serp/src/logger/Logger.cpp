#include "logger/Logger.hpp"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace
{

    void logImpl(const std::function<std::unique_ptr<serp::LogWriter>()> &logger, const char *fmt, va_list args)
    {
        if (!fmt)
            return;

        va_list argsCopy;
        va_copy(argsCopy, args);
        const int size = std::vsnprintf(nullptr, 0, fmt, argsCopy);
        va_end(argsCopy);

        if (size < 0)
            return;

        char *buffer = static_cast<char *>(std::malloc(size + 1));
        if (!buffer)
            return;

        std::vsnprintf(buffer, size + 1, fmt, args);

        logger() << buffer;

        std::free(buffer);
    }

} // namespace

extern "C"
{

    void logVerbose(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        logImpl([]
                { return serp::logVerbose(); }, fmt, args);
        va_end(args);
    }

    void logDebug(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        logImpl([]
                { return serp::logDebug(); }, fmt, args);
        va_end(args);
    }

    void logInfo(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        logImpl([]
                { return serp::logInfo(); }, fmt, args);
        va_end(args);
    }

    void logWarn(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        logImpl([]
                { return serp::logWarn(); }, fmt, args);
        va_end(args);
    }

    void logError(const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        logImpl([]
                { return serp::logError(); }, fmt, args);
        va_end(args);
    }
}
