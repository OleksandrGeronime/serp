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

#include "TestRunner.hpp"

namespace serp
{

    class PromiseTest final
    {
    public:
        explicit PromiseTest(TestRunner &testRunner) : _testRunner(testRunner) {}

        template <typename Value>
        void expectPromise(
            const std::string &line,
            const std::string &cutEventLoop,
            std::shared_ptr<Promise<Value>> promise,
            const std::string &expected,
            const CheckType &checkType = CheckType::EQUAL)
        {
            Async<std::shared_ptr<Promise<Value>>>(
                cutEventLoop,
                [this](const std::shared_ptr<Promise<Value>> &p)
                {
                    p->subscribe([this](uint32_t requestId, const Value &value)
                                 { _testRunner.receive("", requestId, value); });
                },
                promise);

            _testRunner.expect(line, expected, nullptr, checkType);
        }

        template <typename Value>
        void subscribePromise(
            const std::string &cutEventLoop,
            std::shared_ptr<Promise<Value>> promise)
        {
            Async<std::shared_ptr<Promise<Value>>>(
                cutEventLoop,
                [this](const std::shared_ptr<Promise<Value>> &p)
                {
                    p->subscribe([this](uint32_t requestId, const Value &value)
                                 { _testRunner.receive("subscribePromise", requestId, value); });
                },
                promise);
        }

    private:
        TestRunner &_testRunner;
    };

} // namespace serp
