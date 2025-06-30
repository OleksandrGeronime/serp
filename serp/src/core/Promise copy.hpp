#pragma once

#include "PPromise.hpp"
#include "Response.hpp"

namespace serp
{

    template <typename Value>
    class Promise : public PPromise<Value>
    {
    public:
        Promise(const int32_t &PRequestId, const std::string &senderThreadName,
                const std::string &responseReceiverName, const std::string &methodName,
                const LoggingType loggingType)
            : PPromise<Value>(PRequestId, senderThreadName, responseReceiverName, methodName, loggingType),
              mPRequestThread{""},
              mCancelFunc{nullptr},
              mResponse{nullptr},
              mbCanceled{false} {}

        virtual ~Promise() = default;

        Promise(const Promise &) = delete;
        void operator=(const Promise &) = delete;

        uint32_t subscribe(const std::function<void(int32_t, const Value &)> &callback) override
        {
            bool bCanceled = false;
            uint32_t subscribtionId = 0U;
            {
                const std::unique_lock<std::mutex> lock(PPromise<Value>::mMutex);
                bCanceled = mbCanceled;
            }
            if (!bCanceled)
            {
                subscribtionId = PPromise<Value>::subscribe(callback);
            }
            else
            {
                logWarn() << "Promise::subscribe: PRequest already canceled";
            }

            return subscribtionId;
        }

        inline void createResponse()
        {
            const std::unique_lock<std::mutex> lock(PPromise<Value>::mMutex);

            mResponse = std::make_shared<Response<Value>>(
                std::static_pointer_cast<PPromise<Value>>(PPromise<Value>::shared_from_this()));
        }

        inline void setCancelFunc(std::function<void(void)> cancelFunc)
        {
            bool bCanceled = false;

            {
                const std::unique_lock<std::mutex> lock(PPromise<Value>::mMutex);

                mPRequestThread = App::threadName();
                mCancelFunc = cancelFunc;

                bCanceled = mbCanceled;
                mbCanceled = false;
            }

            if (bCanceled)
            {
                cancel();
            }
        }

        inline void cancel()
        {
            const std::unique_lock<std::mutex> lock(PPromise<Value>::mMutex);

            if (!mbCanceled)
            {
                if (!PPromise<Value>::mIsReady)
                {
                    if (mCancelFunc)
                    {
                        App::invoke(mPRequestThread, std::make_shared<Event<>>(mCancelFunc));
                    }
                }
                else
                {
                    logWarn() << "Promise::cancel: PRequest already processed";
                }

                PPromise<Value>::mListeners.clear();
                mbCanceled = true;
            }
            else
            {
                logWarn() << "Promise::cancel: PRequest already canceled";
            }
        }

        inline std::shared_ptr<Response<Value>> getResponse()
        {
            const std::unique_lock<std::mutex> lock(PPromise<Value>::mMutex);

            return mResponse;
        }

    private:
        std::string mPRequestThread;
        std::function<void(void)> mCancelFunc;
        std::shared_ptr<Response<Value>> mResponse;
        bool mbCanceled;
    };

} // namespace serp
