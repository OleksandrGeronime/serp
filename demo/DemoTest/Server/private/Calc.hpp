#pragma once

#include "itc.hpp"

#include "ICalcConsumer.hpp"

namespace Calc
{
    extern const std::string THREAD_NAME;

    void setCalcConsumer(ICalcConsumer* consumer);

    void sum(int a, int b);
    void multiply(int a, int b);
}

DECLARE_STATIC_CALL (CALL_sum, Calc::THREAD_NAME, Calc::sum, int, int)
DECLARE_STATIC_CALL (CALL_multiply, Calc::THREAD_NAME, Calc::multiply, int, int)