#pragma once

#include "App.hpp"
#include "Invoker.hpp"
#include "Invokable.hpp"
#include "logger/Logger.hpp"

namespace serp
{
    class App;

    template <typename ContextPRequest, typename... Args>
    class AsyncCall final : public Invokable
    {
    public:
        AsyncCall(const std::string &senderThreadName,
                  const std::string &methodName,
                  ContextPRequest *const contextPRequest,
                  const std::function<void(ContextPRequest *, Args...)> &PRequest,
                  std::tuple<Args...> &&params,
                  LoggingType loggingType) noexcept
            : _senderThreadName(senderThreadName),
              _methodName(methodName),
              _contextPRequest(contextPRequest),
              _request(PRequest),
              _params(std::move(params)),
              _loggingType(loggingType) {}

        void invoke() const override
        {
            switch (_loggingType)
            {
            case LoggingType::ENABLE:
                itcLog(eLogLevel::info, _senderThreadName, App::threadName(),
                       _methodName, _params);
                break;
            case LoggingType::DISABLE:
                // No logging
                break;
            case LoggingType::WITHOUT_PARAMS:
                itcLog(eLogLevel::info, _senderThreadName, App::threadName(),
                       _methodName, {});
                break;
            }

            invokeAsyncCall(_contextPRequest, _request,
                            typename make_indexes<Args...>::type(),
                            std::move(_params));
        }

    private:
        const std::string _senderThreadName;
        const std::string _methodName;
        ContextPRequest *const _contextPRequest;
        const std::function<void(ContextPRequest *, Args...)> _request;
        mutable std::tuple<Args...> _params;
        const LoggingType _loggingType;
    };

    template <typename... Args>
    class AsyncCallFunc final : public Invokable
    {
    public:
        explicit AsyncCallFunc(const std::function<void(Args...)> &PRequest,
                               std::tuple<Args...> &&params = std::make_tuple<>()) noexcept
            : _request(PRequest),
              _params(std::move(params)) {}

        void invoke() const override
        {
            invokeAsyncCallFunc(_request,
                                typename make_indexes<Args...>::type(),
                                std::move(_params));
        }

    private:
        const std::function<void(Args...)> _request;
        mutable std::tuple<Args...> _params;
    };

} // namespace serp
