#include <iostream>

#include "serp.hpp"

struct S1 : serp::Service {
    static const std::string THREAD;

    S1() : serp::Service("S1T", std::chrono::milliseconds{1000}) {
        serp::logMethod("S1::S1");
    }

    void init(const std::function<void(Service::Status)> reply) override {
        serp::logMethod("S1::init");

        mTimer = serp::Timer::create(std::chrono::seconds(1U), true, [](const serp::TimerPtr& timer) {
            std::cout << "S1::init.mTimer" << std::endl;
            serp::logMethod("S1::init.mTimer");
        });
        mTimer->start();

        reply(serp::Service::Status::SUCCESSFUL);
    }

    void deinit(const std::function<void(Service::Status)> reply) override {
        std::cout << "S1::deinit" << std::endl;
        serp::logMethod("S1::deinit");
        
        mTimer->stop();

        reply(serp::Service::Status::SUCCESSFUL);
    }

    serp::TimerPtr mTimer;
};
const std::string S1::THREAD = "S1T";

int main() {
    // serp::logger::addStrategy<serp::LogStrategyConsole>();

    std::cout << "111" << std::endl;
    serp::logInfo() << "main 0";

    std::cout << "112 " << serp::App::instance().threadName() << std::endl;
    
    auto s1 = std::make_shared<S1>();

    serp::logInfo() << "main 1";

    s1->Init();

    serp::logInfo() << "main 2";

    std::cout << "run" << std::endl;

    serp::App::run();

    s1->destroy();
    return 0;
}