/*
 * Copyright 2025 Oleksandr Geronime
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 */

#include "serp.hpp"

class DemoCommand : public serp::Command
{
    COMMAND

public:
    void onExecute() override
    {
        serp::logMethod("DemoCommand::onExecute");

        finish(serp::ICommand::Result::Success);
    }

    void onPostProcessing() override
    {
        serp::logMethod("DemoCommand::onPostProcessing");
    }

    std::string getName() const override
    {
        return "DemoCmd";
    }
};

struct S1 : public serp::Service
{
    static const std::string THREAD;
    static const std::string INTERFACE;

    S1() : serp::Service("S1T", std::chrono::milliseconds{1000}), mCommandQueue{serp::CommandProcessor::create(serp::CommandProcessor::Type::Queue)}
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

        mCommandQueue->add<DemoCommand>();
    }

    serp::Method::Bind<S1, &S1::foo> Foo{this, "Foo"};

private:
    serp::CommandProcessorPtr mCommandQueue;
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
    std::shared_ptr<serp::Promise<int32_t>> mAddPromise;
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