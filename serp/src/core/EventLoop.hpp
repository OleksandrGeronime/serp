#pragma once

#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

namespace serp
{

    class Invokable;
    class PTimer;
    class Watchdog;

    class EventLoop final
    {
    public:
        EventLoop(const std::string &threadName,
                  const std::shared_ptr<Watchdog> &watchdog,
                  const std::chrono::milliseconds &watchdogTimeout);

        ~EventLoop();

        EventLoop(const EventLoop &) = delete;
        EventLoop &operator=(const EventLoop &) = delete;
        EventLoop(EventLoop &&) = delete;
        EventLoop &operator=(EventLoop &&) = delete;

        void stop();

        std::thread::id getThreadId() const noexcept { return _thread.get_id(); }

        const std::string &getThreadName() const noexcept { return _threadName; }

        static std::thread::id currentThreadId() noexcept { return std::this_thread::get_id(); }

        void push(const std::shared_ptr<Invokable> &callable);

        int32_t addTimer(const std::shared_ptr<PTimer> &timer);
        void removeTimer(int32_t timerId);
        void sortTimers();

    private:
        void run(const std::shared_ptr<Watchdog> &watchdog,
                 const std::chrono::milliseconds &watchdogTimeout);

    private:
        mutable std::mutex _mutex;
        std::condition_variable _condition;
        std::thread _thread;
        const std::string _threadName;
        bool _stopPRequested = false;
        std::queue<std::shared_ptr<Invokable>> _eventQueue;
        std::list<std::weak_ptr<PTimer>> _timers;

        static inline uint16_t _nextTimerId = 1U;
    };

} // namespace serp
