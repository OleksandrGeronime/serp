#include "Client.hpp"

#include <iostream>

#include "../../Server/IServer.hpp"

const std::string IClient::THREAD_NAME = "CLIENT";

Client::Client(IServer* pServer)
: mpServer(pServer)
{
}

void Client::responseSum(int response)
{
    std::cout << "Client::responseSum " << response << std::endl;
}

void Client::responseFactorial(int response)
{
    std::cout << "Client::responseFactorial " << response << std::endl;
}

void Client::responseConvert(int response)
{
    std::cout << "Client::responseConvert " << response << std::endl;
}

void Client::run() 
{
    //itc::invoke(CALL_requestConvert::Call(mpServer, "312"));

    itc::invoke(CALL_requestFactorial::Call(mpServer, 10));
}