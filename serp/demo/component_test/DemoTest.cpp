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