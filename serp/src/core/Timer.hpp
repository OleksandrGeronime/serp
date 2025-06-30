#pragma once

#include <chrono>
#include <functional>
#include <memory>

namespace serp
{

    class Timer
    {
    protected:
        Timer() = default;

    public:
        // Delete copy and move semantics
        Timer(const Timer &) = delete;
        Timer &operator=(const Timer &) = delete;
        Timer(Timer &&) = delete;
        Timer &operator=(Timer &&) = delete;

        virtual ~Timer() = default;

        /**
         * Creates a timer instance and registers it in the EventLoop.
         *
         * @param period Timer duration in milliseconds.
         * @param repeating Whether the timer repeats after expiration.
         * @param callback Function to execute on timer expiration.
         * @return Shared pointer to created Timer, or nullptr on failure.
         */
        static std::shared_ptr<Timer> create(
            const std::chrono::milliseconds &period,
            bool repeating,
            const std::function<void(std::shared_ptr<Timer>)> &callback);

        // Timer control methods
        virtual void start() = 0;
        virtual void stop() = 0;
        virtual void restart() = 0;

        // Timer state query methods
        virtual bool isStarted() const = 0;
        virtual bool isRepeating() const = 0;
        virtual int32_t getId() const = 0;
    };

    using TimerPtr = std::shared_ptr<Timer>;

} // namespace serp
