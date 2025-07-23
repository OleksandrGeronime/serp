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

#include "DependencyMock.hpp"

const std::string DependencyMock::THREAD = "CUTT";
const std::string DependencyMock::INTERFACE = "CUTT";

DependencyMock::DependencyMock(std::shared_ptr<serp::TestRunner> testRunner) : mTestRunner{testRunner}
{
    serp::logMethod("DependencyMock::DependencyMock");
}

void DependencyMock::add(serp::ResponsePtr<int32_t> reply, int32_t i1, int32_t i2)
{
    serp::logMethod("DependencyMock::add", reply, i1, i2);

    reply->call(mTestRunner->receiveWithRet<int32_t>("DependencyMock::add", i1, i2));
}