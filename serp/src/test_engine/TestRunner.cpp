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

#include "TestRunner.hpp"
#include "PromiseTest.hpp"

#include <gtest/gtest.h>
#include <boost/algorithm/string/predicate.hpp>
#include <chrono>
#include <regex>

namespace serp
{

    namespace
    {
        constexpr const char *ANSI_OK_COLOR = "\033[1;32m";
        constexpr const char *ANSI_ERROR_COLOR = "\033[1;31m";
        constexpr const char *ANSI_RESET_COLOR = "\033[0m";
    }

    std::ostream &operator<<(std::ostream &os, const CheckType &value)
    {
        switch (value)
        {
        case CheckType::EQUAL:
            return os << "EQUAL";
        case CheckType::START:
            return os << "START";
        case CheckType::END:
            return os << "END";
        case CheckType::CONTAIN:
            return os << "CONTAIN";
        case CheckType::REGEX:
            return os << "REGEX";
        case CheckType::JSON:
            return os << "JSON";
        default:
            return os << "CheckType::UNKNOWN_TYPE(" << static_cast<std::underlying_type_t<CheckType>>(value) << ")";
        }
    }

    void TestRunner::setUp()
    {
        logMethod("TestRunner::setUp");
        const std::unique_lock<std::mutex> lock(_mutex);
        _finish = false;
        _receivedReady = false;
        _returnedReady = false;
    }

    void TestRunner::tearDown()
    {
        logMethod("TestRunner::tearDown");
        const std::unique_lock<std::mutex> lock(_mutex);
        _finish = true;
        _throwException = false;
        _exception.reset();
        _cvReturned.notify_one();
        _cvReceived.notify_one();
    }

    std::any TestRunner::validate(const std::string &received)
    {
        logMethod("TestRunner::validate", received);

        if (_finish)
            return _returned;

        std::unique_lock<std::mutex> lock(_mutex);

        if (_receivedReady)
            _cvReceived.wait(lock, [this]
                             { return !_receivedReady; });

        _received = received;
        _receivedReady = true;
        _cvReceived.notify_one();

        if (!_returnedReady)
            _cvReturned.wait(lock, [this]
                             { return _returnedReady || _finish; });

        std::any returned = _returned;
        _returnedReady = false;

        if (_throwException)
        {
            logInfo() << "TestRunner::validate throw exception: " << _exception->what();
            auto ex = std::move(_exception);
            _throwException = false;
            throw *ex;
        }

        _cvReturned.notify_one();
        return returned;
    }

    void TestRunner::expect(const std::string &line, const std::string &expected, std::any returned, const CheckType &checkType)
    {
        logMethod("TestRunner::expect", expected, returned.type().name(), checkType);

        if (_finish)
            return;

        std::unique_lock<std::mutex> lock(_mutex);

        if (_exception)
        {
            logInfo() << "TestRunner::expect queued exception: " << _exception->what();
            _throwException = true;
        }

        if (_returnedReady)
            _cvReturned.wait(lock, [this]
                             { return !_returnedReady; });

        _returned = std::move(returned);
        _returnedReady = true;
        _cvReturned.notify_one();

        _cvReceived.wait_for(lock, _timeout, [this]
                             { return _receivedReady; });

        if (!_received.empty())
        {
            const bool valid = isValid(expected, checkType);
            if (!valid)
            {
                printError(line, expected, _received, "");
                EXPECT_EQ(_received, expected);
            }
            else
            {
                logInfo() << ANSI_OK_COLOR << "OK: " << line << " " << expected << ANSI_RESET_COLOR;
            }

            _received.clear();
            _receivedReady = false;
        }
        else
        {
            logError() << ANSI_ERROR_COLOR << "line: " << line;
            logError() << "TIMEOUT " << expected << ANSI_RESET_COLOR;
            throw false;
        }

        _cvReceived.notify_one();
    }

    void TestRunner::expectTrue(const std::string &line, const bool &value)
    {
        if (value)
        {
            logInfo() << ANSI_OK_COLOR << "OK: " << line << " expected: true" << ANSI_RESET_COLOR;
        }
        else
        {
            printError(line, "true", "false", "");
            throw std::string("expectTrue EXIT");
        }
    }

    void TestRunner::expectFalse(const std::string &line, const bool &value)
    {
        if (!value)
        {
            logInfo() << ANSI_OK_COLOR << "OK: " << line << " expected: false" << ANSI_RESET_COLOR;
        }
        else
        {
            printError(line, "false", "true", "");
            throw std::string("expectFalse EXIT");
        }
    }

    void TestRunner::printError(const std::string &line,
                                const std::string &expected,
                                const std::string &received,
                                const std::string &errorMessage)
    {
        logError() << ANSI_ERROR_COLOR << "VALIDATION FAILED at line: " << line << ANSI_RESET_COLOR;
        logError() << ANSI_ERROR_COLOR << "Expected: \"" << expected << "\"" << ANSI_RESET_COLOR;
        logError() << ANSI_ERROR_COLOR << "Received: \"" << received << "\"" << ANSI_RESET_COLOR;
        if (!errorMessage.empty())
            logError() << ANSI_ERROR_COLOR << "Error: \"" << errorMessage << "\"" << ANSI_RESET_COLOR;
    }

    void TestRunner::checkIdle(const std::string &cutThreadName, std::chrono::milliseconds wait /*= std::chrono::milliseconds{100}*/)
    {
        std::this_thread::sleep_for(wait);
        Async<>(cutThreadName, [this]()
                { receive("FINISH"); });
        expect(_CL_, "FINISH()");
    }

    bool TestRunner::isValid(const std::string &expected, const CheckType &checkType)
    {
        bool result = false;
        switch (checkType)
        {
        case CheckType::START:
            result = boost::algorithm::starts_with(_received, expected);
            break;
        case CheckType::END:
            result = boost::algorithm::ends_with(_received, expected);
            break;
        case CheckType::CONTAIN:
            result = boost::algorithm::contains(_received, expected);
            break;
        case CheckType::REGEX:
        {
            std::string prepared = std::regex_replace(expected, std::regex("(\\(|\\))"), "\\$1");
            prepared = std::regex_replace(prepared, std::regex("(\\\\\\\\)"), "");
            result = std::regex_match(_received, std::regex(prepared));
            break;
        }
        case CheckType::JSON:
        {
            static const std::regex ws{"[\\s\\r\\n]+"};
            const auto actual = std::regex_replace(_received, ws, "");
            const auto expectedNorm = std::regex_replace(expected, ws, "");
            result = actual == expectedNorm;
            break;
        }
        case CheckType::EQUAL:
        default:
            result = _received == expected;
            break;
        }
        return result;
    }

} // namespace serp
