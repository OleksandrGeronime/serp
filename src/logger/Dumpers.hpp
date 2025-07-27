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

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <tuple>
#include <vector>
#include <cxxabi.h>

namespace serp
{

    // Tuple printing
    template <class TupType, size_t... Indices>
    void printTuple(std::ostream &out, const TupType &tup, std::index_sequence<Indices...>)
    {
        (..., (out << (Indices == 0 ? "" : ", ") << std::get<Indices>(tup)));
    }

    template <class... Ts>
    void printTuple(std::ostream &out, const std::tuple<Ts...> &tup)
    {
        printTuple(out, tup, std::index_sequence_for<Ts...>{});
    }

    // Structs for tagged logging
    struct log_size_t
    {
        explicit log_size_t(std::size_t value) : _value(value) {}
        std::size_t _value;
    };

    struct log_bool
    {
        explicit log_bool(bool value) : _value(value) {}
        bool _value;
    };

    // STL container streamers
    template <typename T>
    std::ostream &operator<<(std::ostream &out, const std::vector<T> &vec)
    {
        out << "[";
        for (auto it = vec.begin(); it != vec.end(); ++it)
        {
            if (it != vec.begin())
                out << ", ";
            out << *it;
        }
        return out << "]";
    }

    template <typename K, typename V>
    std::ostream &operator<<(std::ostream &out, const std::map<K, V> &map)
    {
        out << "{";
        for (auto it = map.begin(); it != map.end(); ++it)
        {
            if (it != map.begin())
                out << ", ";
            out << *it;
        }
        return out << "}";
    }

    template <typename F, typename S>
    std::ostream &operator<<(std::ostream &out, const std::pair<F, S> &pair)
    {
        return out << "{" << pair.first << ", " << pair.second << "}";
    }

    template <typename... Ts>
    std::ostream &operator<<(std::ostream &out, const std::tuple<Ts...> &tup)
    {
        out << "{";
        printTuple(out, tup);
        return out << "}";
    }

    // Smart pointer streamers
    template <typename T>
    std::ostream &operator<<(std::ostream &out, const std::unique_ptr<T> &ptr)
    {
        if (!ptr)
            return out << "nullptr";
        return out << ptr.get() << "[" << *ptr << "]";
    }

    template <typename T>
    std::ostream &operator<<(std::ostream &out, const std::shared_ptr<T> &ptr)
    {
        if (!ptr)
            return out << "nullptr";
        return out << ptr.get() << "[" << *ptr << "]";
    }

    template <typename T>
    std::ostream &operator<<(std::ostream &out, const std::weak_ptr<T> &weak)
    {
        return out << weak.lock();
    }

    // Function object dumper
    template <typename T>
    std::ostream &operator<<(std::ostream &out, const std::function<T> &fn)
    {
        return out << (fn ? "func()" : "nullfn");
    }

    // Misc types
    std::ostream &operator<<(std::ostream &out, const std::chrono::milliseconds &val);
    std::ostream &operator<<(std::ostream &out, const std::chrono::seconds &val);
    std::ostream &operator<<(std::ostream &out, const std::runtime_error &err);
    std::ostream &operator<<(std::ostream &out, const log_bool &val);

    // Utilities
    inline std::string demangle(const char* name)
    {
        int status = 0;
        char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
        std::string result = (status == 0 && demangled) ? demangled : name;
        std::free(demangled);
        return result;
    }

} // namespace serp
