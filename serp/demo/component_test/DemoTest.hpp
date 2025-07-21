#pragma once

#include "serp.hpp"

#include <gtest/gtest.h>

class DependencyMock;
class CUT;

class DemoTest : public ::testing::Test {
public:
    DemoTest();
    virtual ~DemoTest();

    void SetUp() override;
    void TearDown() override;

    std::shared_ptr<serp::TestRunner> mTestRunner;
    std::shared_ptr<DependencyMock> mDependencyMock;
    std::shared_ptr<CUT> mCUT;
};