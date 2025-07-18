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
