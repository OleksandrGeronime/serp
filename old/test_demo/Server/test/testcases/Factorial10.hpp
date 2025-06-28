#include "../ServerTest.hpp"

TEST_F(ServerTest, Factorial10)
{
    itc::invoke(CALL_requestFactorial::Call(mpServer, 10));
    mpValidator->waitFor("ICalc::multiply(10, 9)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 90));
    mpValidator->waitFor("ICalc::multiply(90, 8)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 720));
    mpValidator->waitFor("ICalc::multiply(720, 7)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 5040));
    mpValidator->waitFor("ICalc::multiply(5040, 6)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 30240));
    mpValidator->waitFor("ICalc::multiply(30240, 5)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 151200));
    mpValidator->waitFor("ICalc::multiply(151200, 4)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 604800));
    mpValidator->waitFor("ICalc::multiply(604800, 3)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 1814400));
    mpValidator->waitFor("ICalc::multiply(1814400, 2)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 3628800));
    mpValidator->waitFor("IClient::responseFactorial(3628800)");
}