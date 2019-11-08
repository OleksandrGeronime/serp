#include "Calc.hpp"

#include <iostream>

namespace Calc
{    
    static ICalcConsumer* gpCalcConsumer = nullptr;

    void setCalcConsumer(ICalcConsumer* consumer)
    {
        std::cout << "Calc::setCalcConsumer " << consumer << std::endl;
        gpCalcConsumer = consumer;
    }

    void sum(int a, int b) 
    {
        if (gpCalcConsumer) {
            itc::invoke(CALL_sumResult::Call(gpCalcConsumer, a + b));
        }
    }

    void multiply(int a, int b)
    {
        if (gpCalcConsumer) {
            itc::invoke(CALL_multiplyResult::Call(gpCalcConsumer, a * b));
        } else {
            std::cerr << "Calc::multiply gpCalcConsumer is nullptr" << std::endl;
        }
    }
}
