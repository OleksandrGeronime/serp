#include "LogWriter.hpp"
#include "LogManager.hpp"

namespace serp
{

    LogWriter::LogWriter(const std::string &threadId, const std::string &prefix, const eLogLevel logLevel)
        : _threadName(threadId), _prefix(prefix), _level(logLevel) {}

    LogWriter::LogWriter(LogWriter &&other) noexcept
        : _baseStream(std::move(other._baseStream)),
          _threadName(std::move(other._threadName)),
          _prefix(std::move(other._prefix)),
          _level(std::exchange(other._level, serp::eLogLevel::error)) {}

    LogWriter::~LogWriter() noexcept
    {
        do
        {
#if defined(LOG_LEVEL)
#if LOG_LEVEL == LOG_LEVEL_OFF
            break;
#elif LOG_LEVEL == LOG_LEVEL_FATAL
            if (eLogLevel::fatal < _level)
                break;
#elif LOG_LEVEL == LOG_LEVEL_ERROR
            if (eLogLevel::error < _level)
                break;
#elif LOG_LEVEL == LOG_LEVEL_WARN
            if (eLogLevel::warn < _level)
                break;
#elif LOG_LEVEL == LOG_LEVEL_INFO
            if (eLogLevel::info < _level)
                break;
#elif LOG_LEVEL == LOG_LEVEL_DEBUG
            if (eLogLevel::debug < _level)
                break;
#elif LOG_LEVEL == LOG_LEVEL_VERBOSE
            if (eLogLevel::verbose < _level)
                break;
#else
            if (eLogLevel::info < _level)
                break;
#endif
#else
            if (eLogLevel::info < _level)
                break;
#endif

            LogManager::getInstance().log(_threadName, _level, _prefix, _baseStream.str());
        } while (false);
    }

    LogWriter &LogWriter::operator=(LogWriter &&other) noexcept
    {
        _baseStream.swap(other._baseStream);
        std::swap(_threadName, other._threadName);
        std::swap(_prefix, other._prefix);
        std::swap(_level, other._level);
        return *this;
    }

} // namespace serp
