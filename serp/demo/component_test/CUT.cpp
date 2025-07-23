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

#include "CUT.hpp"

const std::string CUT::THREAD = "CUTT";
const std::string CUT::INTERFACE = "CUTT";

CUT::CUT(std::shared_ptr<::DependencyMock> dependencyMock) : serp::Service("CUTT", std::chrono::seconds{1}), mDependencyMock{dependencyMock}, mI1{0}, mI2{0}
{
    serp::logMethod("CUT::CUT");
}

void CUT::init(const std::function<void(Service::Status)> reply)
{
    serp::logMethod("CUT::init", reply);

    reply(serp::Service::Status::SUCCESSFUL);
}

void CUT::deinit(const std::function<void(Service::Status)> reply)
{
    serp::logMethod("CUT::deinit", reply);

    reply(serp::Service::Status::SUCCESSFUL);
}

void CUT::runScenario(serp::ResponsePtr<int32_t> reply, Scenario s)
{
    serp::logMethod("CUT::runScenario", reply, s);

    switch (s)
    {
    case Dependency:
        mDependencyMock->Add(mI1, mI2)->subscribe([this, reply](int32_t requestId, const int32_t &result)
                                                  { serp::logMethod("CUT::runScenario::mDependencyMock->Add::reply", requestId, result);
                                                    OnEvent(std::to_string(mI1) + " + " + std::to_string(mI2), result);
                                                    reply->call(result); });

        break;
    case Notification:
        OnEvent("Hello", 42);
        reply->call(42);
        break;
    case Property:
        Counter = Counter + 1;
        reply->call(Counter);
        break;
    default:
        serp::logError() << "Unexpected scenario";
        reply->call(-1);
        break;
    }
}

void CUT::setArgs(int32_t i1, int32_t i2)
{
    serp::logMethod("CUT::setArgs", i1, i2);

    mI1 = i1;
    mI2 = i2;
}

std::ostream &operator<<(std::ostream &out, const CUT::Scenario &value)
{
    switch (value)
    {
    case CUT::Scenario::Notification:
        out << "Notification";
        break;
    case CUT::Scenario::Property:
        out << "Property";
        break;
    case CUT::Scenario::Dependency:
        out << "Dependency";
        break;
    default:
        out << "CUT::Scenario::UNKNOWN_TYPE(" << static_cast<std::underlying_type_t<CUT::Scenario>>(value) << ")";
        break;
    }
    return out;
}