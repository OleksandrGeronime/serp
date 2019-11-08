#pragma once

#include <string>

#include "itc.hpp"

class IClient 
{
public:
    static const std::string THREAD_NAME;

    virtual ~IClient(){};

    virtual void responseSum(int response) = 0;
    virtual void responseFactorial(int response) = 0;
    virtual void responseConvert(int response) = 0;

    virtual void run() = 0;
};

DECLARE_CALL(CALL_responseSum, IClient::THREAD_NAME, IClient, responseSum, int)
DECLARE_CALL(CALL_responseFactorial, IClient::THREAD_NAME, IClient, responseFactorial, int)
DECLARE_CALL(CALL_responseConvert, IClient::THREAD_NAME, IClient, responseConvert, int)

DECLARE_CALL(CALL_run, IClient::THREAD_NAME, IClient, run)