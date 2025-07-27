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

#pragma once

#include "serp.hpp"

#include "DependencyMock.hpp"

class CUT : public serp::Service
{
public:
    static const std::string THREAD;
    static const std::string INTERFACE;

    enum Scenario: int32_t
    {
        Notification,
        Property,
        Dependency
    };

    CUT(std::shared_ptr<::DependencyMock> dependencyMock);

private:
    void init(const std::function<void(Service::Status)> reply) override;
    void deinit(const std::function<void(Service::Status)> reply) override;

    void runScenario(serp::ResponsePtr<int32_t> reply, Scenario s);
    void setArgs(int32_t i1, int32_t i2);

    std::shared_ptr<::DependencyMock> mDependencyMock;
    int32_t mI1;
    int32_t mI2;

public:
    serp::Method::In<Scenario>::Out<int32_t>::Bind<CUT, &CUT::runScenario> RunScenario{this, "RunScenario"};
    serp::Method::In<int32_t, int32_t>::Bind<CUT, &CUT::setArgs> SetArgs{this, "SetArgs"};
    serp::Notification<CUT, std::string, int32_t> OnEvent{"OnEvent"};
    serp::Property<CUT, int32_t> Counter{0, "Counter"};
};

