#pragma once

#include "Invoker.hpp"
#include "Invokable.hpp"
#include "PPromise.hpp"
#include "App.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    template <typename Context, typename Ret, typename... Args>
    class PRequest : public Invokable
    {
    public:
        PRequest(const std::string &sender_thread_name,
                 const std::string &response_receiver_name,
                 const std::string &method_name,
                 Context *const context,
                 const std::function<void(Context *, std::function<void(Ret)>, Args...)> &request,
                 std::tuple<Args...> &&params,
                 LoggingType logging_type)
            : _sender_thread_name{sender_thread_name},
              _response_receiver_name{response_receiver_name},
              _method_name{method_name},
              _context{context},
              _request{request},
              _params{std::move(params)},
              _request_id{getNextRequestId()},
              _promise{std::make_shared<PPromise<Ret>>(_request_id,
                                                       sender_thread_name,
                                                       response_receiver_name,
                                                       method_name,
                                                       logging_type)},
              _logging_type{logging_type} {}

        void invoke() const override
        {
            const std::string &log_source = _response_receiver_name.empty()
                                                ? _sender_thread_name
                                                : _response_receiver_name;

            switch (_logging_type)
            {
            case LoggingType::ENABLE:
                itcLog(eLogLevel::info, log_source, App::currentThreadName(), _method_name, _params);
                break;
            case LoggingType::WITHOUT_PARAMS:
                itcLog(eLogLevel::info, log_source, App::currentThreadName(), _method_name, {});
                break;
            case LoggingType::DISABLE:
            default:
                break;
            }

            invokePRequest(_context,
                           _request,
                           _promise->getReply(),
                           typename make_indexes<Args...>::type(),
                           std::move(_params));
        }

        std::shared_ptr<PPromise<Ret>> getPromise() const
        {
            return _promise;
        }

    protected:
        static uint32_t getNextRequestId()
        {
            std::unique_lock<std::mutex> lock(_request_id_mutex);
            return ++_next_request_id;
        }

        // Data members
        const std::string _sender_thread_name;
        const std::string _response_receiver_name;
        const std::string _method_name;
        Context *const _context;
        const std::function<void(Context *, std::function<void(Ret)>, Args...)> _request;
        mutable std::tuple<Args...> _params;
        const uint32_t _request_id;
        const std::shared_ptr<PPromise<Ret>> _promise;
        const LoggingType _logging_type;

        // Static members
        inline static uint32_t _next_request_id = 0U;
        inline static std::mutex _request_id_mutex;
    };

} // namespace serp
