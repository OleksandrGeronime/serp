#pragma once

#include "itc.hpp"

#include "../IServer.hpp"

class ICalcConsumer
{
public:
    static const std::string THREAD_NAME;

    virtual ~ICalcConsumer(){};

    virtual void sumResult(int sum) = 0;
    virtual void multiplyResult(int multiply) = 0;
};

DECLARE_CALL(CALL_sumResult, IServer::THREAD_NAME, ICalcConsumer, sumResult, int)
DECLARE_CALL(CALL_multiplyResult, IServer::THREAD_NAME, ICalcConsumer, multiplyResult, int)
