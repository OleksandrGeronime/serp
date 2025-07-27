/*
 * Copyright 2025 Oleksandr Geronime
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 */

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

