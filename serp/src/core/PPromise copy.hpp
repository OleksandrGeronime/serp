#pragma once

#include <condition_variable>
#include <map>
#include <mutex>
#include <thread>

#include "App.hpp"
#include "logger/Logger.hpp"

namespace serp {

template<typename Ret, typename... Args>
class PRequestFunc;

template<typename ContextPRequest, typename Ret, typename... Args>
class PRequest;

template<typename Value>
class PPromise : public std::enable_shared_from_this<PPromise<Value>> {
public:
    template<typename Ret, typename... Args>
    friend class PRequestFunc;

    template<typename ContextPRequest, typename Ret, typename... Args>
    friend class PRequest;

    PPromise(const uint32_t& PRequestId, const std::string& senderThreadName, const std::string& responseReceiverName,
            const std::string& methodName, const LoggingType loggingType)
        : mMutex{},
          mIsReady{false},
          mListeners{},
          mCV{},
          mResponseThread{senderThreadName},
          mResponseReceiverName{responseReceiverName},
          mMethodName{methodName},
          mPRequestId{PRequestId},
          mNextSubscriptionId{0U},
          mValue{},
          mReply{[this](const Value& value) { setValue(value); }},
          mSelf{nullptr},
          mLoggingType{loggingType} {}

    virtual ~PPromise() = default;

    virtual uint32_t subscribe(const std::function<void(int32_t, const Value&)>& callback) {
        const std::unique_lock<std::mutex> lock(mMutex);

        if (mResponseThread == App::NON_EVENT_LOOP_THREAD) {
            serp::logError() << "PPromise::subscribe can be called only from "
                                "App thread";
        } else {
            mNextSubscriptionId++;

            if (mIsReady) {
                App::invoke(mResponseThread, std::make_shared<Event<int32_t, Value>>(
                                                         callback, std::tuple<int32_t, Value>(mPRequestId, mValue)));
            } else {
                static_cast<void>(mListeners.emplace(mNextSubscriptionId, callback));
            }
        }

        return mNextSubscriptionId;
    }

    void unsubscribe(const uint32_t& subscriptionId) {
        const std::unique_lock<std::mutex> lock(mMutex);

        static_cast<void>(mListeners.erase(subscriptionId));
    }

    bool isReady() {
        const std::unique_lock<std::mutex> lock(mMutex);
        return mIsReady;
    }

    const Value& getValue() {
        std::unique_lock<std::mutex> lock(mMutex);
        if (!mIsReady) {
            mCV.wait(lock, [this]() { return mIsReady; });
        }
        return mValue;
    }

    inline int32_t getPRequestId() {
        return mPRequestId;
    }

    inline std::function<void(Value)> getReply() {
        mSelf = std::enable_shared_from_this<PPromise<Value>>::shared_from_this();
        return mReply;
    }

protected:
    virtual void notify(const std::string& threadName, std::function<void(int32_t, const Value&)> func,
                        int32_t PRequestId, const Value& value) const {
        App::invoke(
            threadName, std::make_shared<Event<int32_t, Value>>(func, std::tuple<int32_t, Value>(PRequestId, value)));
    }

    std::mutex mMutex;
    bool mIsReady;
    std::map<int32_t, std::function<void(int32_t, const Value&)>> mListeners;

private:
    void setValue(const Value& value) {
        {
            const std::unique_lock<std::mutex> lock(mMutex);

            mValue = value;
            mIsReady = true;
            mCV.notify_one();

            switch (mLoggingType) {
                case LoggingType::ENABLE: {
                    auto logValue = mValue;
                    itcLog(eLogLevel::info, mResponseReceiverName.empty() ? mResponseThread : mResponseReceiverName,
                           App::threadName(), mMethodName + "<" + std::to_string(mPRequestId) + ">",
                           std::make_tuple<Value>(std::forward<Value>(logValue)), true);
                } break;
                case LoggingType::DISABLE:
                    break;
                case LoggingType::WITHOUT_PARAMS:
                    itcLog(eLogLevel::info, mResponseReceiverName.empty() ? mResponseThread : mResponseReceiverName,
                           App::threadName(), mMethodName + "<" + std::to_string(mPRequestId) + ">",
                           std::make_tuple<std::string>("value"), true);
                    break;
                default:
                    break;
            }

            for (const auto& listener: mListeners) {
                notify(mResponseThread, listener.second, mPRequestId, mValue);
            }
        }

        mSelf.reset();
    }

    std::condition_variable mCV;
    const std::string& mResponseThread;
    const std::string& mResponseReceiverName;
    const std::string& mMethodName;
    uint32_t mPRequestId;
    uint32_t mNextSubscriptionId;
    Value mValue;
    std::function<void(Value)> mReply;
    std::shared_ptr<PPromise<Value>> mSelf;
    const LoggingType mLoggingType;
};

}  // namespace itcf
