#pragma once

#include "Server/private/Server.hpp"
#include "Client/private/Client.hpp"
#include "Calc/private/Calc.hpp"

class TestDemo
{
public:
    TestDemo()
    : mpCalc(new Calc())
    , mpServer(new Server(mpCalc))
    , mpClient(new Client(mpServer))
    {
        mpServer->setClient(mpClient);
        mpCalc->setConsumer(mpServer);
    }

    ~TestDemo()
    {
        delete mpCalc;
        delete mpServer;
        delete mpClient;
    }

    void run();

private:
    Calc* mpCalc;
    Server* mpServer;
    Client* mpClient;
};

void TestDemo::run() 
{
    itc::createEventLoop(IServer::THREAD_NAME);
    itc::createEventLoop(IClient::THREAD_NAME);
    itc::createEventLoop(ICalc::THREAD_NAME);

    itc::invoke(CALL_run::Call(mpClient));
}