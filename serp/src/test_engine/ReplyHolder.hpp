#pragma once

#include <any>
#include <mutex>
#include <queue>
#include <functional>
#include <memory>
#include <cstdlib>

#include "serp.hpp"

namespace serp
{
    template <typename... ReplyArgs>
    struct ReplyStub
    {
        const std::function<void(ReplyArgs...)> reply;
        const std::string funcName;

        std::string expectation(const std::string &expectedArgs) const
        {
            std::ostringstream os;
            os << funcName << "(" << expectedArgs << ")";
            return os.str();
        }
    };

    template <>
    struct ReplyStub<void>
    {
        const std::function<void()> reply;
        const std::string expectation;
    };

    class ReplyHolder final
    {
    public:
        static ReplyHolder &getInstance()
        {
            static ReplyHolder instance;
            return instance;
        }

        ~ReplyHolder() = default;

        template <typename ReplyValue>
        std::shared_ptr<Response<ReplyValue>> getFrontReply()
        {
            std::unique_lock lock(_mutex);
            if (_repliesQueue.empty())
            {
                logError() << "ReplyHolder::getFrontReply: replies queue is empty";
                std::exit(1);
            }
            return anyCastFront<std::shared_ptr<Response<ReplyValue>>>(_repliesQueue);
        }

        template <typename ReplyValue>
        void callFrontReply(const ReplyValue &replyValue)
        {
            getFrontReply<ReplyValue>()->call(replyValue);
        }

        void pushReplyFunc(const std::any &replyFunc)
        {
            std::unique_lock lock(_mutex);
            _replyFuncsQueue.push(replyFunc);
        }

        std::function<void()> getFrontReplyFunc()
        {
            std::unique_lock lock(_mutex);
            if (_replyFuncsQueue.empty())
            {
                logError() << "ReplyHolder::getFrontReplyFunc: reply funcs queue is empty";
                std::exit(1);
            }
            return anyCastFront<std::function<void()>>(_replyFuncsQueue);
        }

        template <typename ReplyValue>
        std::function<void(ReplyValue)> getFrontReplyFunc()
        {
            std::unique_lock lock(_mutex);
            if (_replyFuncsQueue.empty())
            {
                logError() << "ReplyHolder::getFrontReplyFunc (templ): reply funcs queue is empty";
                std::exit(1);
            }
            return anyCastFront<std::function<void(ReplyValue)>>(_replyFuncsQueue);
        }

        void clear();

    private:
        ReplyHolder() = default;

        template <typename T>
        T anyCastFront(std::queue<std::any> &queue)
        {
            try
            {
                T val = std::any_cast<T>(queue.front());
                queue.pop();
                return val;
            }
            catch (const std::bad_any_cast &e)
            {
                logError() << "ReplyHolder::anyCastFront bad_any_cast: " << e.what();
                std::exit(1);
            }
        }

        std::mutex _mutex;
        std::queue<std::any> _repliesQueue;
        std::queue<std::any> _replyFuncsQueue;
    };

} // namespace serp
