#pragma once

#include <any>
#include <exception>
#include <mutex>
#include <string>

#include "serp.hpp"

#define _CL_ std::string(::testing::UnitTest::GetInstance()->current_test_info()->name()) + " " + __FILE__ + ":" + std::to_string(__LINE__)

namespace serp
{

    enum class CheckType : uint8_t
    {
        EQUAL = 0,
        START,
        END,
        CONTAIN,
        REGEX,
        JSON
    };

    std::ostream &operator<<(std::ostream &os, const CheckType &value);

    class TestRunner;
    using TestRunnerPtr = std::shared_ptr<TestRunner>;

    class TestRunner final
    {
    public:
        static TestRunnerPtr getInstance()
        {
            static const TestRunnerPtr instance(new TestRunner());
            return instance;
        }

        ~TestRunner() = default;

        void setUp();
        void tearDown();

        void setTimeout(const std::chrono::milliseconds &timeout);
        std::any validate(const std::string &received);

        void expect(
            const std::string &line,
            const std::string &expected,
            std::any returned = {},
            const CheckType &type = CheckType::EQUAL);

        static void expectTrue(const std::string &line, const bool &value);
        static void expectFalse(const std::string &line, const bool &value);

        static void printError(
            const std::string &line,
            const std::string &expected,
            const std::string &received,
            const std::string &errorMessage);

        template <typename... Args>
        static std::string methodToString(const std::string &name, Args &&...args);

        void receive(const std::string &methodName);

        template <typename Arg, typename... Args>
        void receive(const std::string &methodName, Arg &&arg, Args &&...args);

        template <typename Ret>
        Ret receiveWithRet(const std::string &methodName);

        template <typename Ret, typename Arg, typename... Args>
        Ret receiveWithRet(const std::string &methodName, Arg &&arg, Args &&...args);

        void checkIdle(const std::string &cutThreadName, std::chrono::milliseconds wait = std::chrono::milliseconds{100});

    private:
        TestRunner() = default;
        bool isValid(const std::string &expected, const CheckType &type);

        std::mutex _mutex;
        std::condition_variable _cvReceived;
        std::condition_variable _cvReturned;

        bool _finish = false;
        std::chrono::milliseconds _timeout = std::chrono::seconds{1};

        std::string _received;
        bool _receivedReady = false;

        std::any _returned;
        bool _returnedReady = false;

        std::shared_ptr<std::exception> _exception;
        bool _throwException = false;
    };

    // Template definitions

    template <typename... Args>
    std::string TestRunner::methodToString(const std::string &name, Args &&...args)
    {
        std::ostringstream stream;
        stream << name << "(";
        if constexpr (sizeof...(args))
        {
            ([&stream](auto &&first, auto &&...rest)
             {
                stream << first;
                (..., (stream << ", " << rest)); })(std::forward<Args>(args)...);
        }
        stream << ")";
        return stream.str();
    }

    inline void TestRunner::setTimeout(const std::chrono::milliseconds &timeout)
    {
        _timeout = timeout;
    }

    inline void TestRunner::receive(const std::string &methodName)
    {
        (void)validate(methodName + "()");
    }

    template <typename Arg, typename... Args>
    void TestRunner::receive(const std::string &methodName, Arg &&arg, Args &&...args)
    {
        validate(methodToString(methodName, arg, args...));
    }

    template <typename Ret>
    Ret TestRunner::receiveWithRet(const std::string &methodName)
    {
        Ret result{};
        auto ret = validate(methodName + "()");
        try
        {
            result = std::any_cast<Ret>(ret);
        }
        catch (const std::bad_any_cast &e)
        {
            printError("", "", methodName, e.what());
            std::exit(1);
        }
        return result;
    }

    template <typename Ret, typename Arg, typename... Args>
    Ret TestRunner::receiveWithRet(const std::string &methodName, Arg &&arg, Args &&...args)
    {
        Ret result{};
        auto ret = validate(methodToString(methodName, arg, args...));
        try
        {
            result = std::any_cast<Ret>(ret);
        }
        catch (const std::bad_any_cast &e)
        {
            printError("", "", methodName,
                       std::string(e.what()) + ": " + typeid(Ret).name() + " != " + ret.type().name());
            std::exit(1);
        }
        return result;
    }

} // namespace serp
