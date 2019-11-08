#pragma once

#include "../IServer.hpp"
#include "ICalcConsumer.hpp"

class IClient;

class Server: public IServer, public ICalcConsumer
{
public:
    Server();
    virtual ~Server(){}

    // IServer
    void setClient(IClient* pClient) override;
    void requestSum(int a, int b) override;
    void requestFactorial(int a) override;
    void requestConvert(std::string s) override;

    // ICalcConsumer
    void sumResult(int sum) override;
    void multiplyResult(int multiply) override;

private:
    void setCalcConsumer();

    IClient* mpClient;
    int mTmpFactorial;
};