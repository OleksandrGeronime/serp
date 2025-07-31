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

#include <boost/pfr.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>

namespace serp
{

    template <typename T>
    struct is_stl_container : std::false_type
    {
    };

    template <typename... Args>
    struct is_stl_container<std::vector<Args...>> : std::true_type
    {
    };

    template <typename... Args>
    struct is_stl_container<std::list<Args...>> : std::true_type
    {
    };

    template <typename... Args>
    struct is_stl_container<std::set<Args...>> : std::true_type
    {
    };

    template <typename... Args>
    struct is_stl_container<std::map<Args...>> : std::true_type
    {
    };

    template <typename... Args>
    struct is_stl_container<std::unordered_map<Args...>> : std::true_type
    {
    };

    template <typename T>
    T parseArg(const std::string &s);

    template <typename T>
    inline T parseStruct(const std::string &s);

    template <typename T>
    inline T parseArg(const std::string &s)
    {
        if constexpr (is_stl_container<T>::value)
        {
            // TODO: custom parsing std::map, std::vector, etc.
            T val;
            return val;
        }
        else if constexpr (std::is_same_v<T, std::string>)
        {
            if (!s.empty() && s.front() == '"' && s.back() == '"')
            {
                return s.substr(1, s.size() - 2);
            }
            return s;
        }
        else if constexpr (std::is_integral_v<T>)
        {
            T val;
            std::istringstream(s) >> val;
            return val;
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            T val;
            std::istringstream(s) >> val;
            return val;
        }
        else
        {
            return parseStruct<T>(s);
        }
    }

    inline std::string stripBraces(const std::string &s)
    {
        if (s.front() == '{' && s.back() == '}')
            return s.substr(1, s.size() - 2);
        return s;
    }

    inline std::vector<std::string> splitStructFields(const std::string &s)
    {
        std::vector<std::string> result;
        std::string token;
        int brace = 0;
        for (char c : s)
        {
            if (c == ',' && brace == 0)
            {
                result.push_back(token);
                token.clear();
            }
            else
            {
                if (c == '{')
                    ++brace;
                else if (c == '}')
                    --brace;
                token += c;
            }
        }
        if (!token.empty())
            result.push_back(token);
        return result;
    }

    template <typename T>
    inline T parseStruct(const std::string &s)
    {
        std::string body = stripBraces(s);
        std::vector<std::string> fields = splitStructFields(body);

        T obj;
        size_t i = 0;
        boost::pfr::for_each_field(obj, [&](auto &field)
                                   {
        if (i >= fields.size())
            throw std::runtime_error("Not enough fields");
        field = parseArg<std::decay_t<decltype(field)>>(fields[i++]); });
        return obj;
    }

    template <typename T>
    void handleArgument(const std::string &s)
    {
        try
        {
            T value = parseArg<T>(s);
            std::cout << "Parsed value: " << value << std::endl;
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << "Error parsing argument: " << e.what() << std::endl;
        }
    }

    template <typename T>
    auto parseArgWrapper(const std::string &s)
    {
        if constexpr (std::is_class_v<T> && !std::is_same_v<T, std::string>)
        {
            T result;
            std::istringstream iss(s);
            boost::pfr::for_each_field(result, [&iss](auto &field)
                                       {
            std::string field_value;
            iss >> field_value;
            field = parseArgWrapper<std::decay_t<decltype(field)>>(field_value); });
            return result;
        }
        else
        {
            return parseArg<std::decay_t<T>>(s);
        }
    }

    template <typename T>
    std::string toString(const T &val)
    {
        if constexpr (std::is_same_v<T, void>)
        {
            return "OK";
        }
        else
        {
            std::ostringstream oss;
            oss << val;
            return oss.str();
        }
    }

    template <typename Callable, typename... Args, std::size_t... I>
    std::string callWithParsedArgsImpl(Callable &&fn, const std::vector<std::string> &strArgs, std::index_sequence<I...>)
    {
        if constexpr (std::is_same_v<std::invoke_result_t<Callable, Args...>, void>)
        {
            std::invoke(std::forward<Callable>(fn), parseArgWrapper<Args>(strArgs[I])...);
            return "OK";
        }
        else
        {
            auto result = std::invoke(std::forward<Callable>(fn), parseArgWrapper<Args>(strArgs[I])...);
            return toString(result);
        }
    }

    template <typename... Args, typename Callable>
    std::string callWithParsedArgs(Callable &&fn, const std::vector<std::string> &strArgs)
    {
        if (strArgs.size() != sizeof...(Args))
        {
            throw std::runtime_error("Argument count mismatch");
        }
        return callWithParsedArgsImpl<Callable, Args...>(std::forward<Callable>(fn), strArgs,
                                                         std::index_sequence_for<Args...>{});
    }

} // namespace serp