/** @file ServerTest.cpp
 *  @brief Definition of class for testing Server
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#include "ServerTest.hpp"

#include "testcases.hpp"

const std::string IClient::THREAD_NAME = "SERVER_TEST";
const std::string ICalc::THREAD_NAME = "SERVER_TEST";

ServerTest::ServerTest()
: mpValidator(std::make_shared<itc::test::Validator>(2000))
, mpCalc(std::make_shared<CalcStub>(mpValidator))
, mpClient(std::make_shared<ClientStub>(mpValidator))
, mpServer(std::make_shared<Server>(mpCalc))
{
    mpServer->setClient(mpClient);

    itc::createEventLoop(IServer::THREAD_NAME);
    itc::createEventLoop(IClient::THREAD_NAME);
}

void ServerTest::SetUp() 
{

}

void ServerTest::TearDown() 
{

}