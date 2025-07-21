#include "serp.hpp"

struct S1 : serp::Service {
    static const std::string THREAD;

    S1() : serp::Service("S1T", std::chrono::milliseconds{1000}) {
        serp::logMethod("S1::S1");
    }

    void init(const std::function<void(Service::Status)> reply) override {
        serp::logMethod("S1::init", reply);

        mTimer = serp::Timer::create(std::chrono::seconds(1U), true, [](const serp::TimerPtr& timer) {
            serp::logMethod("S1::init.mTimer");//, timer);
        });
        mTimer->start();

        reply(serp::Service::Status::SUCCESSFUL);
    }

    void deinit(const std::function<void(Service::Status)> reply) override {
        serp::logMethod("S1::deinit", reply);
        
        mTimer->stop();

        reply(serp::Service::Status::SUCCESSFUL);
    }

    serp::TimerPtr mTimer;
};
const std::string S1::THREAD = "S1T";

int main() {
    serp::logger::addStrategy<serp::LogStrategyConsole>();
    
    auto s1 = std::make_shared<S1>();

    s1->Init();

    serp::App::run();

    s1->destroy();

    return 0;
}