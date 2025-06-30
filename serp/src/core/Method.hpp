#pragma once

#include "Call.hpp"
#include "Promise.hpp"
#include "Request.hpp"

namespace serp
{
    struct Method final
    {
        template <typename Context, void (Context::*func)()>
        struct Bind final
        {
            Bind(Context *const pContext, const std::string &name = "defaultName",
                 const LoggingType loggingType = LoggingType::ENABLE,
                 const std::string &threadName = Context::THREAD)
                : mContext(pContext), mName(name), mThreadName{threadName}, mLoggingType{loggingType} {}

            ~Bind() = default;
            Bind(const Bind &) = delete;
            Bind &operator=(const Bind &) = delete;
            Bind(Bind &&) = delete;
            Bind &operator=(Bind &&) = delete;

            void operator()()
            {
                const auto &senderThreadName =
                    Context::THREAD == Context::INTERFACE ? App::currentThreadName() : Context::INTERFACE;

                const auto PRequest = std::make_shared<AsyncCall<Context>>(
                    senderThreadName, mName, mContext, func, std::make_tuple(),
                    mLoggingType);

                App::invoke(mThreadName, PRequest);
            }

        private:
            Context *mContext;
            const std::string mName;
            const std::string mThreadName;
            const LoggingType mLoggingType;
        };

        template <typename Ret = void>
        struct Out final
        {
            template <typename Context, void (Context::*func)(std::shared_ptr<Response<Ret>>)>
            struct Bind final
            {
                Bind(Context *const pContext, const std::string &name = "defaultName",
                     const LoggingType loggingType = LoggingType::ENABLE,
                     const std::string &threadName = Context::THREAD)
                    : mContext{pContext}, mName{name}, mThreadName{threadName}, mLoggingType{loggingType}
                {
                }

                ~Bind() = default;
                Bind(const Bind &) = delete;
                Bind &operator=(const Bind &) = delete;
                Bind(Bind &&) = delete;
                Bind &operator=(Bind &&) = delete;

                std::shared_ptr<Promise<Ret>> operator()()
                {
                    const auto &senderThreadName =
                        Context::THREAD == Context::INTERFACE ? App::currentThreadName() : Context::THREAD;

                    const auto PRequest =
                        std::make_shared<Request<Context, Ret>>(
                            senderThreadName, Context::INTERFACE, mName, mContext, func,
                            std::make_tuple(), mLoggingType);

                    App::invoke(mThreadName, PRequest);

                    return PRequest->getPromise();
                }

            private:
                Context *mContext;
                const std::string mName;
                const std::string mThreadName;
                const LoggingType mLoggingType;
            };
        };

        template <typename... Args>
        struct In final
        {
            template <typename Ret = void>
            struct Out final
            {
                template <typename Context, void (Context::*func)(std::shared_ptr<Response<Ret>>, Args...)>
                struct Bind
                {
                    Bind(Context *const pContext, const std::string &name = "defaultName",
                         const LoggingType loggingType = LoggingType::ENABLE,
                         const std::string &threadName = Context::THREAD)
                        : mContext{pContext}, mName{name}, mThreadName{threadName}, mLoggingType{loggingType}
                    {
                    }

                    ~Bind() = default;
                    Bind(const Bind &) = delete;
                    Bind &operator=(const Bind &) = delete;
                    Bind(Bind &&) = delete;
                    Bind &operator=(Bind &&) = delete;

                    std::shared_ptr<Promise<Ret>> operator()(Args... args)
                    {
                        const auto &senderThreadName =
                            Context::THREAD == Context::INTERFACE ? App::currentThreadName() : Context::THREAD;

                        const auto request =
                            std::make_shared<Request<Context, Ret, typename std::decay<Args>::type...>>(
                                senderThreadName, Context::INTERFACE, mName, mContext, func,
                                std::make_tuple(std::forward<Args>(args)...), mLoggingType);

                        App::invoke(mThreadName, request);

                        return request->getPromise();
                    }

                private:
                    Context *mContext;
                    const std::string mName;
                    const std::string mThreadName;
                    const LoggingType mLoggingType;
                };
            };

            template <typename Context, void (Context::*func)(Args...)>
            struct Bind final
            {
                Bind(Context *const pContext, const std::string &name = "defaultName",
                     const LoggingType loggingType = LoggingType::ENABLE,
                     const std::string &threadName = Context::THREAD)
                    : mContext(pContext), mName(name), mThreadName{threadName}, mLoggingType{loggingType} {}

                ~Bind() = default;
                Bind(const Bind &) = delete;
                Bind &operator=(const Bind &) = delete;
                Bind(Bind &&) = delete;
                Bind &operator=(Bind &&) = delete;

                void operator()(Args... args)
                {
                    const auto &senderThreadName =
                        Context::THREAD == Context::INTERFACE ? App::currentThreadName() : Context::INTERFACE;

                    const auto PRequest = std::make_shared<AsyncCall<Context, typename std::decay<Args>::type...>>(
                        senderThreadName, mName, mContext, func, std::make_tuple(std::forward<Args>(args)...),
                        mLoggingType);

                    App::invoke(mThreadName, PRequest);
                }

            private:
                Context *mContext;
                const std::string mName;
                const std::string mThreadName;
                const LoggingType mLoggingType;
            };
        };
    };

} // namespace serp