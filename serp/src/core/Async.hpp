#pragma once

#include "App.hpp"
#include "Event.hpp"

namespace serp
{
    template <typename... Args>
    struct Async final
    {
        Async(const std::function<void(Args...)> &fn, const Args &&...args)
        {
            App::invoke(
                App::currentThreadName(),
                std::make_shared<Event<Args...>>(fn, std::make_tuple(std::forward<const Args>(args)...)));
        }

        Async(const std::string &thread,
              const std::function<void(Args...)> &fn, const Args &&...args)
        {
            App::invoke(
                thread,
                std::make_shared<Event<Args...>>(fn, std::make_tuple(std::forward<const Args>(args)...)));
        }
    };

} // namespace  serp
