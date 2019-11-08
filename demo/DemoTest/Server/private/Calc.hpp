#pragma once

#include "itc.hpp"

#include "ICalcConsumer.hpp"

namespace Calc
{
    void setCalcConsumer(ICalcConsumer* consumer);

    void sum(int a, int b);
    void multiply(int a, int b);
}

DECLARE_STATIC_EVENT (EVENT_sum, Calc::sum, int, int)
DECLARE_STATIC_EVENT (EVENT_multiply, Calc::multiply, int, int)