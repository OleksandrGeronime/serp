#include "Calc.hpp"

#include <iostream>

#include "../ICalcConsumer.hpp"
 
const std::string ICalc::THREAD_NAME = "CALC";

void Calc::setConsumer(std::shared_ptr<ICalcConsumer> pConsumer)
{
    mpConsumer = pConsumer;
}

void Calc::sum(int a, int b) 
{
    if (mpConsumer) {
        itc::invoke(CALL_sumResult::Call(mpConsumer, a + b));
    } else {
        std::cerr << "Calc::sum gpCalcConsumer is nullptr" << std::endl;
    }
}

void Calc::multiply(int a, int b)
{
    if (mpConsumer) {
        itc::invoke(CALL_multiplyResult::Call(mpConsumer, a * b));
    } else {
        std::cerr << "Calc::multiply gpCalcConsumer is nullptr" << std::endl;
    }
}
