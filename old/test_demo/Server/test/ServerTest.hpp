/** @file ServerTest.hpp
 *  @brief Declaration of class for testing Server
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
    ~ServerTest() = default;
    void SetUp() override;
    void TearDown() override;

protected:
    std::shared_ptr<itc::test::Validator> mpValidator;
    std::shared_ptr<ICalc> mpCalc;
    std::shared_ptr<IClient> mpClient;
    std::shared_ptr<Server> mpServer;
};