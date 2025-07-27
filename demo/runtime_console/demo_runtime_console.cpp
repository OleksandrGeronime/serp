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

#include "runtime/RuntimeConsole.hpp"

struct S1 : public serp::Service
{
    static const std::string THREAD;
    static const std::string INTERFACE;

    S1() : serp::Service("S1T", std::chrono::milliseconds{2000})
    {
        serp::logMethod("S1::S1");
    }

    virtual ~S1()
    {
        serp::logMethod("S1::~S1");
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
    }

    void sayHello(std::string name)
    {
        serp::logMethod("S1::sayHello", name);
    }

    void giveMeAnswer(serp::ResponsePtr<int32_t> reply)
    {
        serp::logMethod("S1::giveMeAnswer", reply);

        reply->call(42);
    }

    void add(serp::ResponsePtr<int32_t> reply, int32_t i1, int32_t i2)
    {
        serp::logMethod("S1::add", reply, i1, i2);

        std::this_thread::sleep_for(std::chrono::seconds(1));

        reply->call(i1 + i2);
    }

    serp::Method::Bind<S1, &S1::foo> Foo{this, "Foo"};
    serp::Method::In<std::string>::Bind<S1, &S1::sayHello> SayHello{this, "SayHello"};
    serp::Method::Out<int32_t>::Bind<S1, &S1::giveMeAnswer> GiveMeAnswer{this, "GiveMeAnswer"};
    serp::Method::In<int32_t, int32_t>::Out<int32_t>::Bind<S1, &S1::add> Add{this, "Add"};
};
const std::string S1::THREAD = "S1T";
const std::string S1::INTERFACE = "S1T";

int main()
{
    serp::logger::addStrategy<serp::LogStrategyConsole>();

    auto s1 = std::make_shared<S1>();

    s1->Init();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Use interface methods to simulated calls. For Example:" << std::endl;
    std::cout << "Foo()" << std::endl;
    std::cout << "SayHello(world)" << std::endl;
    std::cout << "GiveMeAnswer()" << std::endl;
    std::cout << "Add(2,3)" << std::endl;

    auto runtime = serp::RuntimeConsole();
    runtime.run(true);

    serp::App::run();

    runtime.stop();

    s1->destroy();

    return 0;
}