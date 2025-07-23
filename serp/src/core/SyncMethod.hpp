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

#include "Call.hpp"
#include "Promise.hpp"
#include "Request.hpp"

namespace serp
{

    struct SyncMethod final
    {
        template <typename... Args>
        struct In final
        {
            template <typename Ret = void>
            struct Out final
            {
                template <typename Context, Ret (Context::*Func)(Args...)>
                struct Bind final
                {
                    Bind(Context *context,
                         const std::string &name = "defaultName",
                         LoggingType logging_type = LoggingType::ENABLE)
                        : _context{context}, _name{name}, _logging_type{logging_type} {}

                    ~Bind() = default;
                    Bind(const Bind &) = delete;
                    Bind &operator=(const Bind &) = delete;
                    Bind(Bind &&) = delete;
                    Bind &operator=(Bind &&) = delete;

                    Ret operator()(Args... args)
                    {
                        logCall(args...);
                        Ret result = (_context->*Func)(std::forward<Args>(args)...);
                        logReturn(result);
                        return std::move(result);
                    }

                private:
                    void logCall(Args &&...args) const
                    {
                        switch (_logging_type)
                        {
                        case LoggingType::ENABLE:
                            itcLog(eLogLevel::info, App::threadName(),
                                   Context::INTERFACE, _name,
                                   std::make_tuple<Args...>(std::forward<Args>(args)...));
                            break;
                        case LoggingType::WITHOUT_PARAMS:
                            itcLog(eLogLevel::info, App::threadName(),
                                   Context::INTERFACE, _name, {});
                            break;
                        case LoggingType::DISABLE:
                        default:
                            break;
                        }
                    }

                    void logReturn(const Ret &result) const
                    {
                        switch (_logging_type)
                        {
                        case LoggingType::ENABLE:
                            itcLog(eLogLevel::info, App::threadName(),
                                   Context::INTERFACE, _name,
                                   std::make_tuple<Ret>(result), true);
                            break;
                        case LoggingType::WITHOUT_PARAMS:
                            itcLog(eLogLevel::info, App::threadName(),
                                   Context::INTERFACE, _name, {}, true);
                            break;
                        case LoggingType::DISABLE:
                        default:
                            break;
                        }
                    }

                    Context *_context;
                    const std::string _name;
                    const LoggingType _logging_type;
                };

                struct Const final
                {
                    template <typename Context, Ret (Context::*Func)(Args...) const>
                    struct Bind final
                    {
                        Bind(Context *context,
                             const std::string &name = "defaultName",
                             LoggingType logging_type = LoggingType::ENABLE)
                            : _context{context}, _name{name}, _logging_type{logging_type} {}

                        ~Bind() = default;
                        Bind(const Bind &) = delete;
                        Bind &operator=(const Bind &) = delete;
                        Bind(Bind &&) = delete;
                        Bind &operator=(Bind &&) = delete;

                        Ret operator()(Args... args)
                        {
                            logCall(args...);
                            Ret result = (_context->*Func)(std::forward<Args>(args)...);
                            logReturn(result);
                            return std::move(result);
                        }

                    private:
                        void logCall(Args &&...args) const
                        {
                            switch (_logging_type)
                            {
                            case LoggingType::ENABLE:
                                itcLog(eLogLevel::info, App::threadName(),
                                       Context::INTERFACE, _name,
                                       std::make_tuple<Args...>(std::forward<Args>(args)...));
                                break;
                            case LoggingType::WITHOUT_PARAMS:
                                itcLog(eLogLevel::info, App::threadName(),
                                       Context::INTERFACE, _name, {});
                                break;
                            case LoggingType::DISABLE:
                            default:
                                break;
                            }
                        }

                        void logReturn(const Ret &result) const
                        {
                            switch (_logging_type)
                            {
                            case LoggingType::ENABLE:
                                itcLog(eLogLevel::info, App::threadName(),
                                       Context::INTERFACE, _name,
                                       std::make_tuple<Ret>(result), true);
                                break;
                            case LoggingType::WITHOUT_PARAMS:
                                itcLog(eLogLevel::info, App::threadName(),
                                       Context::INTERFACE, _name, {}, true);
                                break;
                            case LoggingType::DISABLE:
                            default:
                                break;
                            }
                        }

                        Context *_context;
                        const std::string _name;
                        const LoggingType _logging_type;
                    };
                };
            };

            // Specialization for void return type
            template <typename Context, void (Context::*Func)(Args...)>
            struct Bind final
            {
                Bind(Context *context,
                     const std::string &name = "defaultName",
                     LoggingType logging_type = LoggingType::ENABLE)
                    : _context{context}, _name{name}, _logging_type{logging_type} {}

                ~Bind() = default;
                Bind(const Bind &) = delete;
                Bind &operator=(const Bind &) = delete;
                Bind(Bind &&) = delete;
                Bind &operator=(Bind &&) = delete;

                void operator()(Args... args)
                {
                    switch (_logging_type)
                    {
                    case LoggingType::ENABLE:
                        itcLog(eLogLevel::info, App::threadName(),
                               Context::INTERFACE, _name,
                               std::make_tuple<Args...>(std::forward<Args>(args)...));
                        break;
                    case LoggingType::WITHOUT_PARAMS:
                        itcLog(eLogLevel::info, App::threadName(),
                               Context::INTERFACE, _name, {});
                        break;
                    case LoggingType::DISABLE:
                    default:
                        break;
                    }

                    (_context->*Func)(std::forward<Args>(args)...);

                    switch (_logging_type)
                    {
                    case LoggingType::ENABLE:
                        itcLog(eLogLevel::info, App::threadName(),
                               Context::INTERFACE, _name, std::make_tuple<>(), true);
                        break;
                    case LoggingType::WITHOUT_PARAMS:
                        itcLog(eLogLevel::info, App::threadName(),
                               Context::INTERFACE, _name, {}, true);
                        break;
                    case LoggingType::DISABLE:
                    default:
                        break;
                    }
                }

            private:
                Context *_context;
                const std::string _name;
                const LoggingType _logging_type;
            };
        };
    };

} // namespace serp
