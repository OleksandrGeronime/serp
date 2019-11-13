#pragma once

#include "../IClient.hpp"

class IServer;

class Client: public IClient 
{
public:
    Client(std::shared_ptr<IServer> pServer);
    virtual ~Client() = default;
    
    // IClient
    void responseSum(int response) override;
    void responseFactorial(int response) override;
    void responseConvert(int response) override;

    void run() override;

private:
    std::shared_ptr<IServer> mpServer;
};