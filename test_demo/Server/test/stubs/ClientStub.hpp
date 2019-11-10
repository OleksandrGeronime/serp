
#pragma once

#include <sstream>

#include "../Validator.hpp"
#include "../../../Client/IClient.hpp"

class ClientStub: public IClient
{
public:
    ClientStub(Validator* pValidator): mpValidator(pValidator){}

    void responseSum(int response) override {}
    void responseFactorial(int response) override 
    {
        std::ostringstream stream;
        stream << "IClient::responseFactorial(" << response << ")";
        mpValidator->validate(stream.str()); 
    };
    void responseConvert(int response) override {}

    void run() override {}

private:
    Validator* mpValidator;
};