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
        template <typename... CallArgs>
        AsyncWithDelay(const std::chrono::milliseconds &delay,
                       std::function<void(Args...)> fn, CallArgs &&...args)
            : AsyncWithDelay(App::threadName(), delay, std::move(fn), std::forward<CallArgs>(args)...)
        {
        }

        template <typename... CallArgs>
        AsyncWithDelay(const std::string &thread,
                       const std::chrono::milliseconds &delay,
                       std::function<void(Args...)> fn, CallArgs &&...args)
        {
            Async<Args...>(
                thread,
                [this, delay, fn = std::move(fn)](CallArgs &&...forwarded)
                {
                    const bool oneShot = false;

                    _timer = Timer::create(delay, oneShot,
                                           [this,
                                            fn = std::move(fn),
                                            argsTuple = std::make_tuple(std::forward<CallArgs>(forwarded)...)](const std::shared_ptr<Timer> &) mutable
                                           {
                                               std::apply(fn, std::move(argsTuple));
                                               _timer.reset();
                                           });

                    if (_timer)
                    {
                        _timer->start();
                    }
                    else
                    {
                        serp::logError() << "AsyncWithDelay: Failed to create timer";
                    }
                },
                std::forward<CallArgs>(args)...);
        }

    private:
        std::shared_ptr<Timer> _timer;
    };

} // namespace serp
