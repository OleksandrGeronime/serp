#include "Watchdog.hpp"

#include <algorithm>

#include "App.hpp"
#include "EventLoop.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    void Watchdog::start()
    {
        logMethod("Watchdog::start");

        _stopped = false;

        while (!_stopped)
        {
            std::unique_lock<std::mutex> lock(_mutex);

            // Large timeout by default to avoid busy wait
            auto timeToNextTimeout = std::chrono::milliseconds(1'000'000);

            if (!_startTimes.empty())
            {
                const auto nextTimeoutThread = getNextTimeoutThread();

                if (nextTimeoutThread != std::thread::id{})
                {
                    // Calculate remaining time until timeout for the next thread
                    auto now = std::chrono::steady_clock::now();
                    const auto &startTime = _startTimes[nextTimeoutThread];
                    const auto &timeoutDuration = _timeouts[nextTimeoutThread];

                    timeToNextTimeout = std::chrono::duration_cast<std::chrono::milliseconds>(
                        startTime + timeoutDuration - now);

                    if (timeToNextTimeout <= std::chrono::milliseconds::zero())
                    {
                        logError() << "Watchdog::start: Thread " << nextTimeoutThread << " timeout";

                        _stopped = true;

                        throw App::WatchdogException{nextTimeoutThread};
                    }
                }
            }

            if (!_stopped)
            {
                _cv.wait_for(lock, timeToNextTimeout);
            }
        }
    }

    void Watchdog::stop()
    {
        logMethod("Watchdog::stop");

        {
            std::unique_lock<std::mutex> lock(_mutex);
            _timeouts.clear();
            _startTimes.clear();
            _stopped = true;
        }
        _cv.notify_one();
    }

    void Watchdog::registerThread(
        const std::thread::id &threadId,
        const std::chrono::milliseconds &watchdogTimeout)
    {
        logMethod("Watchdog::registerThread", threadId, watchdogTimeout.count());

        std::unique_lock<std::mutex> lock(_mutex);

        if (watchdogTimeout == std::chrono::milliseconds::zero())
        {
            logWarn() << "Watchdog disabled for thread " << threadId;
        }
        else
        {
            if (_timeouts.count(threadId) > 0)
            {
                logError() << "Watchdog::registerThread: Thread " << threadId
                           << " already registered";
            }
            else
            {
                _timeouts.emplace(threadId, watchdogTimeout);
            }
        }
    }

    void Watchdog::unregisterThread(const std::thread::id &threadId)
    {
        logMethod("Watchdog::unregisterThread", threadId);

        std::unique_lock<std::mutex> lock(_mutex);
        _timeouts.erase(threadId);
        _startTimes.erase(threadId);
    }

    void Watchdog::enterEvent(const std::thread::id &threadId)
    {
        std::unique_lock<std::mutex> lock(_mutex);

        if (_timeouts.count(threadId) > 0)
        {
            if (_startTimes.count(threadId) > 0)
            {
                logError() << "Watchdog::enterEvent: Thread " << threadId
                           << " already processing event";
            }
            else
            {
                _startTimes.emplace(threadId, std::chrono::steady_clock::now());
            }
        }

        _cv.notify_one();
    }

    void Watchdog::exitEvent(const std::thread::id &threadId)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _startTimes.erase(threadId);
    }

    std::thread::id Watchdog::getNextTimeoutThread()
    {
        std::thread::id result;

        auto comp = [this](const auto &lhs, const auto &rhs)
        {
            const auto lhsExpiry = lhs.second + _timeouts[lhs.first];
            const auto rhsExpiry = rhs.second + _timeouts[rhs.first];
            return lhsExpiry < rhsExpiry;
        };

        const auto it = std::min_element(_startTimes.begin(), _startTimes.end(), comp);

        if (it != _startTimes.end())
        {
            result = it->first;
        }

        return result;
    }

} // namespace serp
