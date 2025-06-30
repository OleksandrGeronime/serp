#pragma once

#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

#include "App.hpp"
#include "Event.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    template <typename Ret, typename... Args>
    class PRequestFunc;

    template <typename ContextPRequest, typename Ret, typename... Args>
    class PRequest;

    template <typename Value>
    class PPromise : public std::enable_shared_from_this<PPromise<Value>>
    {
    public:
        template <typename Ret, typename... Args>
        friend class PRequestFunc;

        template <typename ContextPRequest, typename Ret, typename... Args>
        friend class PRequest;

        PPromise(uint32_t request_id,
                 const std::string &sender_thread,
                 const std::string &receiver_thread,
                 const std::string &method_name,
                 LoggingType logging_type)
            : _mutex{},
              _is_ready{false},
              _cv{},
              _response_thread{sender_thread},
              _response_receiver_name{receiver_thread},
              _method_name{method_name},
              _request_id{request_id},
              _next_subscription_id{0U},
              _value{},
              _reply{[this](const Value &value)
                     { setValue(value); }},
              _self{nullptr},
              _logging_type{logging_type} {}

        virtual ~PPromise() = default;

        virtual uint32_t subscribe(const std::function<void(int32_t, const Value &)> &callback)
        {
            std::unique_lock<std::mutex> lock(_mutex);

            if (_response_thread == App::UNKNOWN_THREAD_NAME)
            {
                logError() << "PPromise::subscribe must be called from App thread";
                return 0;
            }

            ++_next_subscription_id;

            if (_is_ready)
            {
                App::invoke(_response_thread, std::make_shared<Event<int32_t, Value>>(
                                                  callback,
                                                  std::make_tuple(_request_id, _value)));
            }
            else
            {
                _listeners.emplace(_next_subscription_id, callback);
            }

            return _next_subscription_id;
        }

        void unsubscribe(uint32_t subscription_id)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            _listeners.erase(subscription_id);
        }

        bool isReady()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            return _is_ready;
        }

        const Value &getValue()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            if (!_is_ready)
            {
                _cv.wait(lock, [this]
                         { return _is_ready; });
            }
            return _value;
        }

        int32_t getRequestId() const
        {
            return _request_id;
        }

        std::function<void(Value)> getReply()
        {
            _self = this->shared_from_this();
            return _reply;
        }

    protected:
        virtual void notify(const std::string &thread_name,
                            const std::function<void(int32_t, const Value &)> &func,
                            int32_t request_id,
                            const Value &value) const
        {
            App::invoke(thread_name, std::make_shared<Event<int32_t, Value>>(
                                         func,
                                         std::make_tuple(request_id, value)));
        }

        mutable std::mutex _mutex;
        bool _is_ready;
        std::map<int32_t, std::function<void(int32_t, const Value &)>> _listeners;

    private:
        void setValue(const Value &value)
        {
            {
                std::unique_lock<std::mutex> lock(_mutex);
                _value = value;
                _is_ready = true;
                _cv.notify_one();

                const auto &thread_name = _response_receiver_name.empty()
                                              ? _response_thread
                                              : _response_receiver_name;

                switch (_logging_type)
                {
                case LoggingType::ENABLE:
                {
                    auto logged_value = _value;
                    itcLog(eLogLevel::info,
                           thread_name,
                           App::currentThreadName(),
                           _method_name + "<" + std::to_string(_request_id) + ">",
                           std::make_tuple<Value>(std::move(logged_value)),
                           true);
                    break;
                }
                case LoggingType::WITHOUT_PARAMS:
                    itcLog(eLogLevel::info,
                           thread_name,
                           App::currentThreadName(),
                           _method_name + "<" + std::to_string(_request_id) + ">",
                           std::make_tuple<std::string>("value"),
                           true);
                    break;
                case LoggingType::DISABLE:
                default:
                    break;
                }

                for (const auto &[_, func] : _listeners)
                {
                    notify(_response_thread, func, _request_id, _value);
                }
            }

            _self.reset();
        }

        std::condition_variable _cv;
        const std::string &_response_thread;
        const std::string &_response_receiver_name;
        const std::string &_method_name;
        uint32_t _request_id;
        uint32_t _next_subscription_id;
        Value _value;
        std::function<void(Value)> _reply;
        std::shared_ptr<PPromise<Value>> _self;
        const LoggingType _logging_type;
    };

} // namespace serp
