#pragma once

#include "PRequest.hpp"
#include "Response.hpp"

namespace serp
{

    template <typename Context, typename Ret, typename... Args>
    class Request final : public PRequest<Context, Ret, Args...>
    {
    public:
        using Base = PRequest<Context, Ret, Args...>;

        Request(const std::string &sender_thread_name,
                const std::string &response_receiver_name,
                const std::string &method_name,
                Context *const context,
                const std::function<void(Context *, std::shared_ptr<Response<Ret>>, Args...)> &request,
                std::tuple<Args...> &&params,
                LoggingType logging_type)
            : Base(sender_thread_name, response_receiver_name, method_name, context, [](Context *, std::function<void(Ret)>, Args...) {}, std::move(params), logging_type), _request(request), _promise(std::make_shared<Promise<Ret>>(Base::_request_id, sender_thread_name, response_receiver_name, method_name, logging_type))
        {
            _promise->createResponse();
        }

        void invoke() const override
        {
            const auto &base = *this;

            const std::string &log_source = base._response_receiver_name.empty()
                                                ? base._sender_thread_name
                                                : base._response_receiver_name;

            const std::string log_method = base._method_name + "<" + std::to_string(base._request_id) + ">";

            switch (base._logging_type)
            {
            case LoggingType::ENABLE:
                itcLog(eLogLevel::info, log_source, App::currentThreadName(), log_method, base._params);
                break;
            case LoggingType::WITHOUT_PARAMS:
                itcLog(eLogLevel::info, log_source, App::currentThreadName(), log_method, {});
                break;
            case LoggingType::DISABLE:
            default:
                break;
            }

            invokeRequest(base._context,
                          _request,
                          _promise->getResponse(),
                          typename make_indexes<Args...>::type(),
                          std::forward<std::tuple<Args...>>(base._params));
        }

        std::shared_ptr<Promise<Ret>> getPromise() const
        {
            return _promise;
        }

    private:
        const std::function<void(Context *, std::shared_ptr<Response<Ret>>, Args...)> _request;
        const std::shared_ptr<Promise<Ret>> _promise;
    };

} // namespace serp
