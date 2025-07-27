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