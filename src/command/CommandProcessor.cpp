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

#include "command/CommandProcessor.hpp"

#include "CommandPool.hpp"
#include "CommandQueue.hpp"

namespace serp
{

    CommandProcessorPtr CommandProcessor::create(Type type)
    {
        switch (type)
        {
        case Type::Queue:
            return std::make_shared<CommandQueue>();
        case Type::Pool:
            return std::make_shared<CommandPool>();
        default:
            serp::logError() << "Unexpected CommandProcessor type: " << type
                             << ". Creating default CommandQueue.";
            return std::make_shared<CommandQueue>();
        }
    }

    CommandProcessor::CommandProcessor()
        : _clearSubscriptionId{0U},
          _listeners{},
          _nextSubscriptionId{0U},
          _blocked{false} {}

    void CommandProcessor::clear(std::function<void(void)> callback)
    {
        logMethod("CommandProcessor::clear");

        if (_clearSubscriptionId == 0U)
        {
            if (size() > 0)
            {
                _clearSubscriptionId = subscribe(
                    CommandProcessor::NotificationType::Finished,
                    [this, callback](
                        const CommandProcessor::NotificationType & /*notificationType*/,
                        const CommandProcessorPtr & /*commandProcessor*/)
                    {
                        logMethod("CommandProcessor::clear::subscribe callback");
                        unsubscribe(_clearSubscriptionId);
                        _clearSubscriptionId = 0U;
                        if (callback)
                        {
                            callback();
                        }
                    });

                clear();
            }
            else
            {
                Async<>([this, callback]()
                        {
                logMethod("CommandProcessor::clear::async callback");
                if (callback) {
                    callback();
                } });
            }
        }
        else
        {
            logError() << "Clear already in progress";
        }
    }

    void CommandProcessor::block()
    {
        logMethod("CommandProcessor::block");
        _blocked = true;
    }

    void CommandProcessor::unblock()
    {
        logMethod("CommandProcessor::unblock");
        _blocked = false;
    }

    uint32_t CommandProcessor::subscribe(
        CommandProcessor::NotificationType notificationType,
        std::function<void(const NotificationType &notificationType,
                           const CommandProcessorPtr &commandProcessor)>
            listener)
    {
        ++_nextSubscriptionId;
        _listeners[notificationType][_nextSubscriptionId] = std::move(listener);
        return _nextSubscriptionId;
    }

    void CommandProcessor::unsubscribe(const uint32_t &subscriptionId)
    {
        std::set<CommandProcessor::NotificationType> typesToRemove;
        for (auto &[type, listeners] : _listeners)
        {
            listeners.erase(subscriptionId);
            if (listeners.empty())
            {
                typesToRemove.insert(type);
            }
        }

        for (const auto &type : typesToRemove)
        {
            _listeners.erase(type);
        }
    }

    size_t CommandProcessor::clear(std::set<int32_t> types) const
    {
        size_t count = 0U;
        for (const auto &type : types)
        {
            count += clear([&type](const CommandPtr &cmd)
                           { return type == cmd->getTypeId(); });
        }
        return count;
    }

    void CommandProcessor::doNotifyListeners(CommandProcessor::NotificationType type)
    {
        if (_listeners.count(type) != 0)
        {
            for (const auto &[subscriptionId, listener] : _listeners[type])
            {
                Async<
                    std::function<void(const NotificationType &, const CommandProcessorPtr &)>,
                    CommandProcessor::NotificationType,
                    CommandProcessorPtr>(
                    [](const std::function<void(const NotificationType &, const CommandProcessorPtr &)> &listener,
                       const CommandProcessor::NotificationType notiType,
                       const CommandProcessorPtr commandProcessor)
                    {
                        listener(notiType, commandProcessor);
                    },
                    listener, type, shared_from_this());
            }
        }
    }

    std::ostream &operator<<(std::ostream &out, const CommandProcessor::Type &value)
    {
        switch (value)
        {
        case CommandProcessor::Type::Queue:
            out << "Queue";
            break;
        case CommandProcessor::Type::Pool:
            out << "Pool";
            break;
        default:
            out << "CommandProcessor::Type::UNKNOWN_TYPE("
                << static_cast<std::underlying_type_t<CommandProcessor::Type>>(value) << ")";
            break;
        }
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const CommandProcessor::NotificationType &value)
    {
        switch (value)
        {
        case CommandProcessor::NotificationType::Changed:
            out << "Changed";
            break;
        case CommandProcessor::NotificationType::Cleared:
            out << "Cleared";
            break;
        case CommandProcessor::NotificationType::Finished:
            out << "Finished";
            break;
        default:
            out << "CommandProcessor::NotificationType::UNKNOWN_TYPE("
                << static_cast<std::underlying_type_t<CommandProcessor::NotificationType>>(value) << ")";
            break;
        }
        return out;
    }

} // namespace serp
