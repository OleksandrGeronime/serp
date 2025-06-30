#pragma once

#include <map>
#include <mutex>
#include <sstream>
#include <thread>
#include <functional>

#include "App.hpp"
#include "logger/Logger.hpp"

namespace serp {

template <typename Context, typename Type>
class Property final {
    struct Subscription {
        Subscription(const std::string& name,
                     std::function<void(const Type&)> callback)
            : thread(name), handler(std::move(callback)) {}

        const std::string& thread;
        std::function<void(const Type&)> handler;
    };

public:
    explicit Property(const std::string& name = "")
        : _value{},
          _ownerThread(Context::THREAD),
          _interface(Context::INTERFACE),
          _name(name),
          _nextId(1) {}

    Property(const Type& initial, const std::string& name = "")
        : _value(initial),
          _ownerThread(Context::THREAD),
          _interface(Context::INTERFACE),
          _name(name),
          _nextId(1) {}

    ~Property() = default;

    Property(const Property&) = delete;
    Property& operator=(const Property&) = delete;
    Property(Property&&) = delete;
    Property& operator=(Property&&) = delete;

    Property& operator=(const Type& val) {
        const auto& current = App::threadName();
        if (current != _ownerThread && _ownerThread == _interface) {
            logError() << "Attempt to assign from thread '" << current
                       << "' to Property owned by '" << _ownerThread << "'";
            return *this;
        }

        std::lock_guard guard(_lock);
        _value = val;
        broadcast();
        return *this;
    }

    operator Type() {
        if (App::threadName() != _ownerThread) {
            std::lock_guard guard(_lock);
            return _value;
        }
        return _value;
    }

    void mutate(std::function<void(Type&)> callback) {
        App::invoke(_ownerThread, std::make_shared<Event<>>(
            [this, callback = std::move(callback)]() mutable {
                std::lock_guard guard(_lock);
                callback(_value);
                broadcast();
            }
        ));
    }

    uint32_t connect(std::function<void(const Type&)> cb) {
        std::lock_guard guard(_lock);

        const uint32_t id = _nextId++;

        std::stringstream msg;
        msg << "assigned id = " << id;

        const auto& threadId = std::this_thread::get_id();
        const bool serverSide = App::instance().getLoopById(threadId) != nullptr;

        if (serverSide) {
            itcLog(eLogLevel::info, App::threadName(), _interface,
                   "A_" + _name + "_subscribe", std::make_tuple(msg.str()));
        } else {
            itcLog(eLogLevel::info, _interface, _ownerThread,
                   "A_" + _name + "_subscribe", std::make_tuple(msg.str()));
        }

        _subscribers.emplace(id, Subscription(App::threadName(), std::move(cb)));
        return id;
    }

    void disconnect(uint32_t id) {
        std::lock_guard guard(_lock);

        const auto& threadId = std::this_thread::get_id();
        const bool serverSide = App::instance().getLoopById(threadId) != nullptr;

        if (serverSide) {
            itcLog(eLogLevel::info, App::threadName(), _interface,
                   "A_" + _name + "_unsubscribe", std::make_tuple(id));
        } else {
            itcLog(eLogLevel::info, _interface, _ownerThread,
                   "A_" + _name + "_unsubscribe", std::make_tuple(id));
        }

        _subscribers.erase(id);
    }

private:
    void broadcast() {
        for (const auto& [id, subscriber] : _subscribers) {
            std::string destination = subscriber.thread.empty() || 
                                      subscriber.thread == App::NON_EVENT_LOOP_THREAD
                                      ? App::threadName()
                                      : subscriber.thread;

            const auto& threadId = std::this_thread::get_id();
            const bool serverSide = App::instance().getLoopById(threadId) != nullptr;

            if (serverSide) {
                itcLog(eLogLevel::info, _interface, destination, "A_" + _name,
                       std::make_tuple(_value), true);
            } else {
                itcLog(eLogLevel::info, destination, _interface, "A_" + _name,
                       std::make_tuple(_value), true);
            }

            propagate(destination, subscriber.handler, _value);
        }
    }

    void propagate(const std::string& destination,
                   const std::function<void(const Type&)>& fn,
                   const Type& val) const {
        App::invoke(
            destination, std::make_shared<Event<Type>>(fn, val));
    }

    Type _value;
    std::map<uint32_t, Subscription> _subscribers;
    uint32_t _nextId;

    const std::string& _ownerThread;
    const std::string& _interface;
    const std::string _name;

    mutable std::mutex _lock;
};

} // namespace serp
