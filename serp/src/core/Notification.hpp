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

#include <map>
#include <mutex>
#include <shared_mutex>
#include <sstream>
#include <thread>
#include <functional>

#include "App.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    template <typename Context, typename... Args>
    class Notification final
    {
    private:
        struct Subscription
        {
            std::string thread;
            std::function<void(Args...)> handler;

            Subscription(std::string threadName, std::function<void(Args...)> cb)
                : thread(std::move(threadName)), handler(std::move(cb)) {}
        };

    public:
        explicit Notification(std::string name = "")
            : _name(std::move(name)),
              _nextId(1),
              _ownerThread(Context::THREAD),
              _interface(Context::INTERFACE) {}

        Notification(const Notification &) = delete;
        Notification &operator=(const Notification &) = delete;
        Notification(Notification &&) = delete;
        Notification &operator=(Notification &&) = delete;
        ~Notification() = default;

        /// Returns number of active listeners
        size_t size() const noexcept
        {
            std::shared_lock lock(_mutex);
            return _subscribers.size();
        }

        /// Connects a listener, returns its subscription ID
        uint32_t connect(std::function<void(Args...)> callback)
        {
            const std::string thread = App::threadName();

            uint32_t id;
            {
                std::unique_lock lock(_mutex);
                id = _nextId++;
                _subscribers.emplace(id, Subscription(thread, std::move(callback)));
            }

            logConnection(true, id);
            return id;
        }

        /// Disconnects a listener by ID
        void disconnect(uint32_t id) noexcept
        {
            {
                std::unique_lock lock(_mutex);
                _subscribers.erase(id);
            }
            logConnection(false, id);
        }

        /// Sends notification to all listeners
        void broadcast(const Args &...args)
        {
            if (App::threadName() != _ownerThread &&
                _ownerThread == _interface)
            {
                logError() << "Notification is allowed only from thread '" << _ownerThread
                           << "', but called from '" << App::threadName() << "'";
                return;
            }

            std::map<uint32_t, Subscription> copy;
            {
                std::shared_lock lock(_mutex);
                copy = _subscribers;
            }

            for (const auto &[id, sub] : copy)
            {
                std::string targetThread = !sub.thread.empty() &&
                                                   sub.thread != App::NON_EVENT_LOOP_THREAD
                                               ? sub.thread
                                               : App::threadName();

                logEvent(targetThread, args...);
                dispatch(targetThread, sub.handler, args...);
            }
        }

        /// Shortcut for broadcast
        template <typename... CallArgs>
        void operator()(CallArgs&&... args)
        {
            broadcast(std::forward<CallArgs>(args)...);
        }

    private:
        void logConnection(bool connecting, uint32_t id) const
        {
            const std::string &current = App::threadName();
            const std::string direction = connecting ? "_subscribe" : "_unsubscribe";
            std::string logName = "N_" + _name + direction;
            std::string idStr = std::to_string(id);

            const bool isServer = App::instance().getLoopById(std::this_thread::get_id()) != nullptr;

            if (isServer)
            {
                itcLog(eLogLevel::info, current, _interface, logName, std::make_tuple(idStr));
            }
            else
            {
                itcLog(eLogLevel::info, _interface, _ownerThread, logName, std::make_tuple(idStr));
            }
        }

        template <typename... Ts>
        void logEvent(const std::string &receiver, const Ts &...args) const
        {
            std::string logName = "N_" + _name;
            const bool isServer = App::instance().getLoopById(std::this_thread::get_id()) != nullptr;

            if (isServer)
            {
                itcLog(eLogLevel::info, _interface, receiver, logName, std::make_tuple(args...), true);
            }
            else
            {
                itcLog(eLogLevel::info, receiver, _interface, logName, std::make_tuple(args...), true);
            }
        }

        void dispatch(const std::string &thread,
                      const std::function<void(Args...)> &fn,
                      const Args &...args) const
        {
            App::invoke(
                thread,
                std::make_shared<Event<Args...>>(fn, std::make_tuple(args...)));
        }

    private:
        mutable std::shared_mutex _mutex;
        std::map<uint32_t, Subscription> _subscribers;
        uint32_t _nextId;

        const std::string &_ownerThread;
        const std::string &_interface;
        std::string _name;
    };

} // namespace serp
