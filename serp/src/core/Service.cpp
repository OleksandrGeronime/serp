#include "Service.hpp"

#include <iostream>

namespace serp
{

    const std::string Service::THREAD = "";
    const std::string Service::INTERFACE = Service::THREAD;

    Service::Service(const std::string &threadName, const std::chrono::milliseconds &watchdogTimeout)
        : mThreadName{threadName},
          mIsInited{false},
          Deinit{this, "Service::deinit", LoggingType::ENABLE, threadName},
          Init{this, "Service::init", LoggingType::ENABLE, threadName}
    {
        logMethod("Service::Service", threadName, watchdogTimeout);

        App::createEventLoop(mThreadName, watchdogTimeout);
    }

    Service::~Service()
    {
        logMethod("Service::~Service");
    }

    std::string Service::getInterfaceName()
    {
        return Service::THREAD;
    }

    void Service::_init(const std::shared_ptr<serp::Response<Service::Status>> reply)
    {
        if (!mIsInited)
        {
            init([reply](const Service::Status &status)
                 { reply->call(status); });
            mIsInited = true;
        }
        else
        {
            logWarn() << "Service " << Service::THREAD << " already inited";
            reply->call(Service::Status::SUCCESSFUL);
        }
    }

    void Service::_deinit(const std::shared_ptr<serp::Response<Service::Status>> reply)
    {
        if (mIsInited)
        {
            deinit([reply](const Service::Status &status)
                   { reply->call(status); });
            mIsInited = false;
        }
        else
        {
            logWarn() << "Service " << Service::THREAD << " already deinited";
            reply->call(Service::Status::SUCCESSFUL);
        }
    }

    void Service::destroy()
    {
        try
        {
            logMethod("Service::destroy");

            if (Deinit()->getValue() == Service::Status::SUCCESSFUL)
            {
                App::stopEventLoop(mThreadName);
            }
            else
            {
                logError() << mThreadName << " failed to deinit and can't be destroyed";
            }
        }
        catch (std::exception &ex)
        {
            std::cerr << "Exception in Service destroy: " << ex.what();
        }
    }

    std::ostream &operator<<(std::ostream &out, const Service::Status &value)
    {
        switch (value)
        {
        case Service::Status::SUCCESSFUL:
            out << "Service::Status::SUCCESSFUL";
            break;
        case Service::Status::FAILED:
            out << "Service::Status::FAILED";
            break;
        default:
            out << "Service::Status::UNKNOWN_TYPE("
                << static_cast<std::uint32_t>(static_cast<std::underlying_type_t<Service::Status>>(value)) << ")";
            break;
        }

        return out;
    }

} // namespace serp
