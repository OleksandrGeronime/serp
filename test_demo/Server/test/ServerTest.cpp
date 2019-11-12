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
: mpValidator(new itc::test::Validator(2000))
, mpCalc(new CalcStub(mpValidator))
, mpClient(new ClientStub(mpValidator))
, mpServer(new Server(mpCalc))
{
    mpServer->setClient(mpClient);

    itc::createEventLoop(IServer::THREAD_NAME);
    itc::createEventLoop(IClient::THREAD_NAME);
}

ServerTest::~ServerTest() 
{
    // delete mpServer;
    // delete mpClient;
    // delete mpCalc;
    // delete mpValidator;
}

void ServerTest::SetUp() 
{

}

void ServerTest::TearDown() 
{

}