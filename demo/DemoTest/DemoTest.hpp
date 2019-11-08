#pragma once

#include "Server/private/Server.hpp"
#include "Client/private/Client.hpp"

class DemoTest
{
public:
    DemoTest()
    : mpServer(new Server())
    , mpClient(new Client(mpServer))
    {
        mpServer->setClient(mpClient);
    }

    ~DemoTest()
    {
        delete mpServer;
        delete mpClient;
    }

    void run();

private:
    IServer* mpServer;
    IClient* mpClient;
};

void DemoTest::run() 
{
    itc::createEventLoop(IServer::THREAD_NAME);
    itc::createEventLoop(IClient::THREAD_NAME);

    mpClient->run();
}