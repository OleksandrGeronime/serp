#pragma once

#include <functional>
#include <string>

#include "Method.hpp"

namespace serp
{

    class Service
    {
    public:
        enum class Status : uint8_t
        {
            SUCCESSFUL,
            FAILED
        };

        static const std::string THREAD;
        static const std::string INTERFACE;

        Service(const std::string &threadName,
                const std::chrono::milliseconds &watchdogTimeout);
        virtual ~Service();

        virtual std::string getInterfaceName();

        template <typename T>
        static std::shared_ptr<T> create()
        {
            return std::make_shared<T>();
        }

        template <typename T, typename... Args>
        static std::shared_ptr<T> create(Args... args)
        {
            return std::make_shared<T>(std::forward<Args>(args)...);
        }

        void destroy();

    private:
        void _init(const std::shared_ptr<serp::Response<Service::Status>> reply);
        void _deinit(
            const std::shared_ptr<serp::Response<Service::Status>> reply);

        virtual void init(const std::function<void(Service::Status)> reply) = 0;
        virtual void deinit(const std::function<void(Service::Status)> reply) = 0;

        const std::string mThreadName;
        bool mIsInited;

        serp::Method::In<>::Out<Service::Status>::Bind<Service,
                                                       &Service::_deinit>
            Deinit;

    public:
        serp::Method::In<>::Out<Service::Status>::Bind<Service,
                                                       &Service::_init>
            Init;
    };

    std::ostream &operator<<(std::ostream &out, const Service::Status &value);

} // namespace serp