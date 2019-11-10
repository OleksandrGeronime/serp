#pragma once

#include <sstream>

#include "../Validator.hpp"
#include "../../../Calc/ICalc.hpp"

class CalcStub: public ICalc
{
public:
    CalcStub(Validator* pValidator): mpValidator(pValidator){}

    void setConsumer(ICalcConsumer* pConsumer) override{};
    void sum(int a, int b) override{};
    void multiply(int a, int b) override
    {
        std::ostringstream stream;
        stream << "ICalc::multiply(" << a << ", " << b << ")";
        mpValidator->validate(stream.str()); 
    };

private:
    Validator* mpValidator;
};