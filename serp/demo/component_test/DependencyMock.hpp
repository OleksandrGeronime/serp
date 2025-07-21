#pragma once

#include "serp.hpp"

class DependencyMock
{
public:
    static const std::string THREAD;
    static const std::string INTERFACE;

    DependencyMock(std::shared_ptr<serp::TestRunner> mTestRunner);

private:
    void add(serp::ResponsePtr<int32_t> reply, int32_t i1, int32_t i2);

    std::shared_ptr<serp::TestRunner> mTestRunner;

public:
    serp::Method::In<int32_t, int32_t>::Out<int32_t>::Bind<DependencyMock, &DependencyMock::add> Add{this, "Add"};
};

