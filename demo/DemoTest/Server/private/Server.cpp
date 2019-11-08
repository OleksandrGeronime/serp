#include "Server.hpp"

#include <string>
#include <iostream>

#include "Calc.hpp"

const std::string IServer::THREAD_NAME = "IServer_thread";

void Server::setClient(IClient* pClient)
{
    mpClient = pClient;
}

void Server::requestSum(int a, int b)
{
    itc::invoke(EVENT_sum::Event(a, b));
}

void Server::requestFactorial(int a)
{
    mTmpFactorial = a - 2;
    itc::invoke(EVENT_multiply::Event(a, a - 1));
}

void Server::requestConvert(std::string s)
{
    itc::invoke(CALL_responseConvert::Call(mpClient, std::stoi(s)));
}



void Server::sumResult(int sum)
{
    itc::invoke(CALL_responseSum::Call(mpClient, sum));
}
 
void Server::multiplyResult(int multiply)
{   
    if (mTmpFactorial > 1) {
        itc::invoke(EVENT_multiply::Event(multiply, mTmpFactorial));
        mTmpFactorial--;
    } else {
        itc::invoke(CALL_responseFactorial::Call(mpClient, multiply));
    }
}



void Server::setCalcConsumer()
{
    std::cout << "Server::setCalcConsumer " << this << std::endl;
    Calc::setCalcConsumer(this);
}