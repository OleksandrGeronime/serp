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
                            itcLog(eLogLevel::info, App::currentThreadName(),
                                   Context::INTERFACE, _name,
                                   std::make_tuple<Args...>(std::forward<Args>(args)...));
                            break;
                        case LoggingType::WITHOUT_PARAMS:
                            itcLog(eLogLevel::info, App::currentThreadName(),
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
                            itcLog(eLogLevel::info, App::currentThreadName(),
                                   Context::INTERFACE, _name,
                                   std::make_tuple<Ret>(result), true);
                            break;
                        case LoggingType::WITHOUT_PARAMS:
                            itcLog(eLogLevel::info, App::currentThreadName(),
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
                                itcLog(eLogLevel::info, App::currentThreadName(),
                                       Context::INTERFACE, _name,
                                       std::make_tuple<Args...>(std::forward<Args>(args)...));
                                break;
                            case LoggingType::WITHOUT_PARAMS:
                                itcLog(eLogLevel::info, App::currentThreadName(),
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
                                itcLog(eLogLevel::info, App::currentThreadName(),
                                       Context::INTERFACE, _name,
                                       std::make_tuple<Ret>(result), true);
                                break;
                            case LoggingType::WITHOUT_PARAMS:
                                itcLog(eLogLevel::info, App::currentThreadName(),
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
                        itcLog(eLogLevel::info, App::currentThreadName(),
                               Context::INTERFACE, _name,
                               std::make_tuple<Args...>(std::forward<Args>(args)...));
                        break;
                    case LoggingType::WITHOUT_PARAMS:
                        itcLog(eLogLevel::info, App::currentThreadName(),
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
                        itcLog(eLogLevel::info, App::currentThreadName(),
                               Context::INTERFACE, _name, std::make_tuple<>(), true);
                        break;
                    case LoggingType::WITHOUT_PARAMS:
                        itcLog(eLogLevel::info, App::currentThreadName(),
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
