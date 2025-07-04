#include "Server.hpp"

#include <string>
#include <iostream>

#include "../../Calc/ICalc.hpp"
#include "../../Client/IClient.hpp"

const std::string IServer::THREAD_NAME = "SERVER";

Server::Server(std::shared_ptr<ICalc> pCalc)
: mpCalc(pCalc)
{  
}

void Server::setClient(std::shared_ptr<IClient> pClient)
{
    mpClient = pClient;
}

void Server::requestSum(int a, int b)
{
    itc::invoke(CALL_sum::Call(mpCalc, a, b));
}

void Server::requestFactorial(int a)
{
    mTmpFactorial = a - 2;
    itc::invoke(CALL_multiply::Call(mpCalc, a, a - 1));
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
        itc::invoke(CALL_multiply::Call(mpCalc, multiply, mTmpFactorial));
        mTmpFactorial--;
    } else {
        itc::invoke(CALL_responseFactorial::Call(mpClient, multiply));
    }
}
