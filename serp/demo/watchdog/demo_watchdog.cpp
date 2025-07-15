#include <iostream>

#include "serp.hpp"

struct S1 : serp::Service
{
    static const std::string THREAD;
    static const std::string INTERFACE;

    S1() : serp::Service("S1T", std::chrono::seconds{1})
    {
        serp::logMethod("S1::S1");
    }

    void init(const std::function<void(Service::Status)> reply) override
    {
        serp::logMethod("S1::init", reply);

        // Sleep for 2 seconds, watchdog timeout for this service is 1 second, so application will crash by watchdog
        std::this_thread::sleep_for(std::chrono::seconds{2U});

        reply(serp::Service::Status::SUCCESSFUL);
    }

    void deinit(const std::function<void(Service::Status)> reply) override
    {
        serp::logMethod("S1::deinit", reply);

        reply(serp::Service::Status::SUCCESSFUL);
    }
};
const std::string S1::THREAD = "S1T";
const std::string S1::INTERFACE = "S1T";

int main()
{
    serp::logger::addStrategy<serp::LogStrategyConsole>();

    auto s1 = std::make_shared<S1>();

    s1->Init();

    serp::App::run();

    s1->destroy();

    return 0;
}