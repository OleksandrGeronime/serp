#pragma once

#include "App.hpp"
#include "Event.hpp"

namespace serp
{
    template <typename... Args>
    struct Async final
    {
        template <typename... CallArgs>
        Async(std::function<void(Args...)> fn, CallArgs &&...args)
        {
            App::invoke(
                App::threadName(),
                std::make_shared<Event<Args...>>(std::move(fn), std::make_tuple(std::forward<CallArgs>(args)...)));
        }

        template <typename... CallArgs>
        Async(const std::string &thread, std::function<void(Args...)> fn, CallArgs &&...args)
        {
            App::invoke(
                thread,
                std::make_shared<Event<Args...>>(std::move(fn), std::make_tuple(std::forward<CallArgs>(args)...)));
        }
    };

} // namespace  serp
