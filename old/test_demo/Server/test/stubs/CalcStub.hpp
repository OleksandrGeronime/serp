#pragma once

#include <sstream>

#include "itctest/Validator.hpp"
#include "../../../Calc/ICalc.hpp"

class CalcStub: public ICalc
{
public:
    CalcStub(std::shared_ptr<itc::test::Validator> pValidator): mpValidator(pValidator){}

    void setConsumer(std::shared_ptr<ICalcConsumer> pConsumer) override{};
    void sum(int a, int b) override{};
    void multiply(int a, int b) override
    {
        std::ostringstream stream;
        stream << "ICalc::multiply(" << a << ", " << b << ")";
        mpValidator->validate(stream.str()); 
    };

private:
    std::shared_ptr<itc::test::Validator> mpValidator;
};