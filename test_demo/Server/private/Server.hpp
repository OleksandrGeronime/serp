#pragma once

#include "../IServer.hpp"
#include "../../Calc/ICalcConsumer.hpp"

class ICalc;
class IClient;

class Server: public IServer, public ICalcConsumer
{
public:
    Server(ICalc* pCalc);
    virtual ~Server();

    // IServer
    void setClient(IClient* pClient) override;
    void requestSum(int a, int b) override;
    void requestFactorial(int a) override;
    void requestConvert(std::string s) override;

    // ICalcConsumer
    void sumResult(int sum) override;
    void multiplyResult(int multiply) override;

private:
    ICalc* mpCalc;
    IClient* mpClient;
    int mTmpFactorial;
};