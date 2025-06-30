#pragma once

#include "Invoker.hpp"
#include "Invokable.hpp"

namespace serp
{

    template <typename... Args>
    class Event final : public Invokable
    {
    public:
        explicit Event(const std::function<void(Args...)> &func,
                       const std::tuple<Args...> &params = std::tuple<>())
            : _func(func), _params(params) {}

        void invoke() const override
        {
            invokeAsyncCallFunc(
                _func, typename make_indexes<Args...>::type(), std::move(_params));
        }

    private:
        const std::function<void(Args...)> _func;
        mutable std::tuple<Args...> _params;
    };

} // namespace serp
