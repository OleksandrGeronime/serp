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

#include <unordered_map>
#include <atomic>
#include <thread>

#include "MethodParser.hpp"

namespace serp
{
    class Runtime
    {
    public:
        virtual void run(bool runInBackground) = 0;

        void stop();

        template <typename Bind>
        static void registerMethod(Bind *bind, const std::string &mName);

        template <typename Bind>
        static void registerMethodOut(Bind *bind, const std::string &mName);

        template <typename Bind, typename... Args>
        static void registerMethodInOut(Bind *bind, const std::string &mName);

        template <typename Bind, typename... Args>
        static void registerMethodIn(Bind *bind, const std::string &mName);

    protected:
        std::thread _thread;
        std::atomic_bool _stop;
        
        static std::unordered_map<std::string, std::function<std::string(const std::vector<std::string> &)>> sMethodMap;
    };

    template <typename Bind>
    void Runtime::registerMethod(Bind *bind, const std::string &mName)
    {
        sMethodMap[mName] = [bind](const std::vector<std::string> &args) -> std::string
        {
            callWithParsedArgs(
                [bind]()
                {
                    (*bind)();
                },
                std::vector<std::string>());
            return "";
        };
    }

    template <typename Bind>
    void Runtime::registerMethodOut(Bind *bind, const std::string &mName)
    {
        sMethodMap[mName] = [bind](const std::vector<std::string> &args) -> std::string
        {
            return callWithParsedArgs(
                [bind]() -> std::string
                {
                    auto result = (*bind)();
                    std::ostringstream oss;
                    oss << result->getValue();
                    return oss.str();
                },
                std::vector<std::string>());
        };
    }

    template <typename Bind, typename... Args>
    void Runtime::registerMethodInOut(Bind *bind, const std::string &mName)
    {
        sMethodMap[mName] = [bind](const std::vector<std::string> &args) -> std::string
        {
            return callWithParsedArgs<Args...>(
                [bind](Args... unpacked) -> std::string
                {
                    auto result = (*bind)(unpacked...);
                    std::ostringstream oss;
                    oss << result->getValue();
                    return oss.str();
                },
                args);
        };
    }

    template <typename Bind, typename... Args>
    void Runtime::registerMethodIn(Bind *bind, const std::string &mName)
    {
        sMethodMap[mName] = [bind](const std::vector<std::string> &args) -> std::string
        {
            callWithParsedArgs<Args...>(
                [bind](Args... unpacked)
                {
                    (*bind)(unpacked...);
                },
                args);
            return "";
        };
    }

} // namespace serp