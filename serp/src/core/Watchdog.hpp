#pragma once

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

namespace serp
{

    /**
     * @brief Monitors registered threads for timeout violations using watchdog timers.
     */
    class Watchdog final
    {
    public:
        Watchdog() = default;
        virtual ~Watchdog() = default;

        void start();
        void stop();

        void registerThread(const std::thread::id &threadId,
                            const std::chrono::milliseconds &timeout);
        void unregisterThread(const std::thread::id &threadId);
        void enterEvent(const std::thread::id &threadId);
        void exitEvent(const std::thread::id &threadId);

    private:
        Watchdog(const Watchdog &) = delete;
        Watchdog &operator=(const Watchdog &) = delete;
        Watchdog(Watchdog &&) = delete;
        Watchdog &operator=(Watchdog &&) = delete;

        std::thread::id getNextTimeoutThread();

        std::mutex _mutex;
        std::condition_variable _cv;
        std::atomic_bool _stopped{false};

        std::map<std::thread::id, std::chrono::milliseconds> _timeouts;
        std::map<std::thread::id, std::chrono::steady_clock::time_point> _startTimes;
    };

} // namespace serp
