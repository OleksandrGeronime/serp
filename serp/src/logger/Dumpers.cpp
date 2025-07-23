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

#include "Dumpers.hpp"

namespace serp
{

    std::ostream &operator<<(std::ostream &out, const std::chrono::milliseconds &value)
    {
        return out << value.count() << "ms";
    }

    std::ostream &operator<<(std::ostream &out, const std::chrono::seconds &value)
    {
        return out << value.count() << "s";
    }

    std::ostream &operator<<(std::ostream &out, const std::runtime_error &value)
    {
        return out << "error(" << value.what() << ")";
    }

    std::ostream &operator<<(std::ostream &out, const log_bool &value)
    {
        return out << (value._value ? "true" : "false");
    }

} // namespace serp
