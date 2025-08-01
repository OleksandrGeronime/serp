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
#include "DemoTest.hpp"
#include "CUT.hpp"
#include "DependencyMock.hpp"

DemoTest::DemoTest() : mTestRunner(serp::TestRunner::getInstance())
{
    static std::once_flag onceFlag;
    std::call_once(onceFlag, []()
                   { serp::logger::addStrategy<serp::LogStrategyConsole>(); });

    serp::logMethod("DemoTest::DemoTest");
}

DemoTest::~DemoTest()
{
    serp::logMethod("DemoTest::~DemoTest");
}

void DemoTest::SetUp()
{
    serp::logMethod("DemoTest::SetUp");

    mTestRunner->setUp();

    mDependencyMock = std::make_shared<DependencyMock>(mTestRunner);
    mCUT = std::make_shared<CUT>(mDependencyMock);
}

void DemoTest::TearDown()
{
    serp::logMethod("DemoTest::TearDown");

    mCUT->destroy();
    mCUT.reset();
    mDependencyMock.reset();

    mTestRunner->tearDown();

    std::this_thread::sleep_for(std::chrono::milliseconds(100));
}