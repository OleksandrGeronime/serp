#pragma once

#include "itc.hpp"

#include "../ICalc.hpp"

class ICalcConsumer;

class Calc: public ICalc
{
public:
    Calc() = default;
    virtual ~Calc() = default;

    // ICalc
    void setConsumer(std::shared_ptr<ICalcConsumer> pConsumer) override;
    void sum(int a, int b) override;
    void multiply(int a, int b) override;

private:
    std::shared_ptr<ICalcConsumer> mpConsumer;
};