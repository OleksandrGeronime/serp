#pragma once

#include "PPromise.hpp"
#include "Response.hpp"

namespace serp
{

    template <typename Value>
    class Promise final : public PPromise<Value>
    {
    public:
        Promise(int32_t request_id,
                const std::string &sender_thread_name,
                const std::string &response_receiver_name,
                const std::string &method_name,
                LoggingType logging_type)
            : PPromise<Value>(request_id, sender_thread_name, response_receiver_name, method_name, logging_type),
              _request_thread{},
              _cancel_func{nullptr},
              _response{nullptr},
              _canceled{false} {}

        ~Promise() override = default;

        Promise(const Promise &) = delete;
        Promise &operator=(const Promise &) = delete;

        uint32_t subscribe(const std::function<void(int32_t, const Value &)> &callback) override
        {
            bool canceled = false;
            {
                std::unique_lock<std::mutex> lock(this->_mutex);
                canceled = _canceled;
            }

            if (canceled)
            {
                logWarn() << "Promise::subscribe: request already canceled";
                return 0;
            }

            return PPromise<Value>::subscribe(callback);
        }

        void createResponse()
        {
            std::unique_lock<std::mutex> lock(this->_mutex);
            _response = std::make_shared<Response<Value>>(this->shared_from_this());
        }

        void setCancelFunc(std::function<void()> cancel_func)
        {
            bool was_canceled = false;

            {
                std::unique_lock<std::mutex> lock(this->_mutex);
                _request_thread = App::currentThreadName();
                _cancel_func = std::move(cancel_func);
                was_canceled = _canceled;
                _canceled = false;
            }

            if (was_canceled)
            {
                cancel();
            }
        }

        void cancel()
        {
            std::unique_lock<std::mutex> lock(this->_mutex);

            if (_canceled)
            {
                logWarn() << "Promise::cancel: request already canceled";
                return;
            }

            if (!this->_is_ready && _cancel_func)
            {
                App::invoke(_request_thread, std::make_shared<Event<>>(_cancel_func));
            }
            else if (this->_is_ready)
            {
                logWarn() << "Promise::cancel: request already processed";
            }

            this->_listeners.clear();
            _canceled = true;
        }

        std::shared_ptr<Response<Value>> getResponse()
        {
            std::unique_lock<std::mutex> lock(this->_mutex);
            return _response;
        }

    private:
        std::string _request_thread;
        std::function<void()> _cancel_func;
        std::shared_ptr<Response<Value>> _response;
        bool _canceled;
    };

} // namespace serp
