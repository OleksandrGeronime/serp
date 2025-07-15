#pragma once

#include <functional>
#include <memory>

#include "logger/Logger.hpp"

namespace serp
{

    template <typename Value>
    class PPromise;

    template <typename Ret>
    class Response
    {
    public:
        Response(const std::shared_ptr<PPromise<Ret>> &promise)
            : mPromise(promise),
              mReply(promise->getReply()) {}

        virtual ~Response() = default;

        inline void setCancelFunc(std::function<void(void)> cancelFunc)
        {
            if (auto PPromise = mPromise.lock())
            {
                PPromise->setCancelFunc(cancelFunc);
            }
            else
            {
                logError() << "Response::setCancelFunc mPromise is empty";
            }
        }

        inline void operator()(const Ret ret) const { mReply(ret); }

        inline void call(const Ret ret) const { mReply(ret); }

    private:
        std::weak_ptr<PPromise<Ret>> mPromise;
        std::function<void(Ret)> mReply;
    };

    template <typename Ret>
    using ResponsePtr = std::shared_ptr<Response<Ret>>;

    template <typename Ret>
    std::ostream &operator<<(std::ostream &out, const Response<Ret> &resp) {
        return out << "Response<" << demangle(typeid(Ret).name()) << ">";
    }

} // namespace serp
