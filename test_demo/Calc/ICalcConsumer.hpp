#pragma once

#include "itc.hpp"

#include "../Server/IServer.hpp"

class ICalcConsumer
{
public:
    virtual ~ICalcConsumer() = default;

    virtual void sumResult(int sum) = 0;
    virtual void multiplyResult(int multiply) = 0;
};

DECLARE_CALL(CALL_sumResult, IServer::THREAD_NAME, ICalcConsumer, sumResult, int)
DECLARE_CALL(CALL_multiplyResult, IServer::THREAD_NAME, ICalcConsumer, multiplyResult, int)
