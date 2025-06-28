#pragma once

#include "../IServer.hpp"
#include "../../Calc/ICalcConsumer.hpp"

class ICalc;
class IClient;

class Server: public IServer, public ICalcConsumer
{
public:
    Server(std::shared_ptr<ICalc> pCalc);
    virtual ~Server() = default;

    // IServer
    void setClient(std::shared_ptr<IClient> pClient) override;
    void requestSum(int a, int b) override;
    void requestFactorial(int a) override;
    void requestConvert(std::string s) override;

    // ICalcConsumer
    void sumResult(int sum) override;
    void multiplyResult(int multiply) override;

private:
    std::shared_ptr<ICalc> mpCalc;
    std::shared_ptr<IClient> mpClient;
    int mTmpFactorial;
};