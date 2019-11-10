#pragma once

#include "itc.hpp"

class ICalcConsumer;

class ICalc
{
public:
    static const std::string THREAD_NAME;

    ICalc() = default;
    virtual ~ICalc() = default;

    virtual void setConsumer(ICalcConsumer* pConsumer) = 0;

    virtual void sum(int a, int b) = 0;
    virtual void multiply(int a, int b) = 0;
};

DECLARE_CALL (CALL_sum, ICalc::THREAD_NAME, ICalc, sum, int, int)
DECLARE_CALL (CALL_multiply, ICalc::THREAD_NAME, ICalc, multiply, int, int)