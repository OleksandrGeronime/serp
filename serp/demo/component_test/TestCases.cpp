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

#include "DemoTest.hpp"
#include "CUT.hpp"

#include <climits>

void subscribe(std::shared_ptr<serp::TestRunner> testRunner, std::shared_ptr<CUT> cut)
{
    cut->OnEvent.connect([testRunner](const std::string &str, int32_t i)
                         { testRunner->receive("OnEvent.connect", str, i); });

    cut->Counter.connect([testRunner](int32_t i)
                         { testRunner->receive("Counter.connect", i); });
}

TEST_F(DemoTest, tc001_01_DemoTest_Notification)
{
    subscribe(mTestRunner, mCUT);
    mCUT->Init();

    mCUT->RunScenario(CUT::Scenario::Notification);
    mTestRunner->expect(_CL_, "OnEvent.connect(Hello, 42)");

    mTestRunner->checkIdle(CUT::THREAD);
}

TEST_F(DemoTest, tc001_02_DemoTest_Property)
{
    subscribe(mTestRunner, mCUT);
    mCUT->Init();

    mCUT->RunScenario(CUT::Scenario::Property);
    mTestRunner->expect(_CL_, "Counter.connect(1)");

    mCUT->RunScenario(CUT::Scenario::Property);
    mTestRunner->expect(_CL_, "Counter.connect(2)");

    mCUT->RunScenario(CUT::Scenario::Property);
    mTestRunner->expect(_CL_, "Counter.connect(3)");

    mTestRunner->checkIdle(CUT::THREAD);
}

TEST_F(DemoTest, tc001_03_DemoTest_Dependency)
{
    subscribe(mTestRunner, mCUT);
    mCUT->Init();

    mCUT->SetArgs(2, 3);
    mCUT->RunScenario(CUT::Scenario::Dependency);
    mTestRunner->expect(_CL_, "DependencyMock::add(2, 3)", 5);
    mTestRunner->expect(_CL_, "OnEvent.connect(2 + 3, 5)");

    mCUT->SetArgs(123, 321);
    mCUT->RunScenario(CUT::Scenario::Dependency);
    mTestRunner->expect(_CL_, "DependencyMock::add(123, 321)", 444);
    mTestRunner->expect(_CL_, "OnEvent.connect(123 + 321, 444)");

    mCUT->SetArgs(INT_MAX, INT_MIN);
    mCUT->RunScenario(CUT::Scenario::Dependency);
    mTestRunner->expect(_CL_, "DependencyMock::add(2147483647, -2147483648)", -1);
    mTestRunner->expect(_CL_, "OnEvent.connect(2147483647 + -2147483648, -1)");

    mTestRunner->checkIdle(CUT::THREAD);
}