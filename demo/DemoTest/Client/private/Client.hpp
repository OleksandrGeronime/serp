#pragma once

#include "../IClient.hpp"

class IServer;

class Client: public IClient 
{
public:
    Client(IServer* pServer);
    virtual ~Client(){}
    
    // IClient
    void responseSum(int response) override;
    void responseFactorial(int response) override;
    void responseConvert(int response) override;

    void run() override;

private:
    IServer* mpServer;
};