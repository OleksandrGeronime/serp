#pragma once

#include "Server/private/Server.hpp"
#include "Client/private/Client.hpp"
#include "Calc/private/Calc.hpp"

class TestDemo
{
public:
    TestDemo()
    : mpCalc(std::make_shared<Calc>())
    , mpServer(std::make_shared<Server>(mpCalc))
    , mpClient(std::make_shared<Client>(mpServer))
    {
        mpServer->setClient(mpClient);
        mpCalc->setConsumer(mpServer);
    }

    ~TestDemo() = default;

    void run();

private:
    std::shared_ptr<Calc> mpCalc;
    std::shared_ptr<Server> mpServer;
    std::shared_ptr<Client> mpClient;
};

void TestDemo::run() 
{
    itc::createEventLoop(IServer::THREAD_NAME);
    itc::createEventLoop(IClient::THREAD_NAME);
    itc::createEventLoop(ICalc::THREAD_NAME);

    itc::invoke(CALL_run::Call(mpClient));
}