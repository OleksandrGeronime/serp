#pragma once

#include <string>

#include "itc.hpp"

class IClient;

class IServer
{
public:
    static const std::string THREAD_NAME;

    virtual ~IServer() = default;

    virtual void setClient(std::shared_ptr<IClient> pClient) = 0;

    virtual void requestSum(int a, int b) = 0;
    virtual void requestFactorial(int a) = 0;
    virtual void requestConvert(std::string s) = 0;      
};

DECLARE_CALL(CALL_requestSum, IServer::THREAD_NAME, IServer, requestSum, int, int)
DECLARE_CALL(CALL_requestFactorial, IServer::THREAD_NAME, IServer, requestFactorial, int)
DECLARE_CALL(CALL_requestConvert, IServer::THREAD_NAME, IServer, requestConvert, std::string)