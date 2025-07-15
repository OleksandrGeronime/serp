#include <iostream>

#include "serp.hpp"

struct S1 : public serp::Service
{
    static const std::string THREAD;
    static const std::string INTERFACE;

    S1() : serp::Service("S1T", std::chrono::milliseconds{1000})
    {
        serp::logMethod("S1::S1");
    }

    void init(const std::function<void(Service::Status)> reply) override
    {
        serp::logMethod("S1::init", reply);

        reply(serp::Service::Status::SUCCESSFUL);
    }

    void deinit(const std::function<void(Service::Status)> reply) override
    {
        serp::logMethod("S1::deinit", reply);

        reply(serp::Service::Status::SUCCESSFUL);
    }

    void foo()
    {
        serp::logMethod("S1::foo");

        OnEvent("Hello", 42);
    }

    void add(serp::ResponsePtr<int> reply, int i1, int i2)
    {
        serp::logMethod("S1::add", reply, i1, i2);

        reply->call(1);
    }

    serp::Method::Bind<S1, &S1::foo> Foo{this, "Foo"};

    serp::Notification<S1, std::string, int> OnEvent{"OnEvent"};
};
const std::string S1::THREAD = "S1T";
const std::string S1::INTERFACE = "S1T";

struct S2 : public serp::Service
{
    static const std::string THREAD;
    static const std::string INTERFACE;

    S2(std::shared_ptr<S1> s1) : serp::Service("S2T", std::chrono::milliseconds{1000}), mS1{s1}
    {
        serp::logMethod("S2::S2");
    }

    void init(const std::function<void(Service::Status)> reply) override
    {
        serp::logMethod("S2::init", reply);

        mS1->OnEvent.connect([](const std::string &value1, const int &value2)
                             { serp::logMethod("S2::init::Counter.subscribe", value1, value2); });

        mS1->Foo();
        mS1->Foo();
        mS1->Foo();
        mS1->Foo();

        reply(serp::Service::Status::SUCCESSFUL);
    }

    void deinit(const std::function<void(Service::Status)> reply) override
    {
        serp::logMethod("S2::deinit", reply);

        reply(serp::Service::Status::SUCCESSFUL);
    }

private:
    std::shared_ptr<S1> mS1;
};
const std::string S2::THREAD = "S2T";
const std::string S2::INTERFACE = "S2T";

int main()
{
    serp::logger::addStrategy<serp::LogStrategyConsole>();

    auto s1 = std::make_shared<S1>();
    auto s2 = std::make_shared<S2>(s1);

    s1->Init();
    s2->Init();

    serp::App::run();

    s2->destroy();
    s1->destroy();

    return 0;
}