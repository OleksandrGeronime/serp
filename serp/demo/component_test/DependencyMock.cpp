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