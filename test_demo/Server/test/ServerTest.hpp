/** @file ServerTest.hpp
 *  @brief Header file to include all test cases
 *
 *  Just include your test case here to run it
 *
 *  @date 2019
 *  @author Alexander Geronime
 */

#pragma once

#include <gtest/gtest.h>

#include "itc.hpp"

#include "../private/Server.hpp"
#include "stubs/CalcStub.hpp"
#include "stubs/ClientStub.hpp"


class ServerTest: public ::testing::Test
{
public:
    ServerTest();
    ~ServerTest();
    void SetUp() override;
    void TearDown() override;

protected:
    itc::test::Validator* mpValidator;
    ICalc* mpCalc;
    IClient* mpClient;
    Server* mpServer;
};