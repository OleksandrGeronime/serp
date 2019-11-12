#include "../ServerTest.hpp"

TEST_F(ServerTest, Factorial5) 
{
    itc::invoke(CALL_requestFactorial::Call(mpServer, 5));
    mpValidator->waitFor("ICalc::multiply(5, 4)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 20));
    mpValidator->waitFor("ICalc::multiply(20, 3)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 60));
    mpValidator->waitFor("ICalc::multiply(60, 2)");
    itc::invoke(CALL_multiplyResult::Call(mpServer, 120));
    mpValidator->waitFor("IClient::responseFactorial(120)");
}