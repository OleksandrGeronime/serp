#pragma once

#include "Timer.hpp"
#include "Async.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    template <typename... Args>
    class AsyncWithDelay final
    {
    public:
        AsyncWithDelay(const std::chrono::milliseconds &delay,
                       const std::function<void(Args...)> &fn, Args &&...args)
            : AsyncWithDelay(App::threadName(), delay, fn, std::forward<Args>(args)...) {}

        AsyncWithDelay(const std::string &thread,
                       const std::chrono::milliseconds &delay,
                       const std::function<void(Args...)> &fn, Args &&...args)
        {
            Async<Args...>(
                thread,
                [this, delay, fn](Args &&...forwarded)
                {
                    const bool oneShot = false;
                    _timer = Timer::create(delay, oneShot,
                                           [this, fn, args = std::make_tuple(std::forward<Args>(forwarded)...)](const std::shared_ptr<Timer> &) mutable
                                           {
                                               std::apply(fn, std::move(args));
                                               _timer.reset();
                                           });

                    if (_timer)
                    {
                        _timer->start();
                    }
                    else
                    {
                        serp::logError() << "asyncWithDelay: Failed to create timer";
                    }
                },
                std::forward<Args>(args)...);
        }

    private:
        std::shared_ptr<Timer> _timer;
    };

} // namespace  serp
