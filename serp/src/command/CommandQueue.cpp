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

#include "CommandQueue.hpp"

#include <algorithm>

#include "command/ICommand.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    CommandQueue::CommandQueue() : _commands{} {}

    CommandQueue::~CommandQueue() = default;

    void CommandQueue::add(CommandPtr command)
    {
        logMethod("CommandQueue::add", command);

        if (_clearSubscriptionId != 0U)
        {
            logWarn() << "Queue clearing is in progress, command is canceled";
            command->cancel();
            return;
        }

        command->subscribe(ICommand::NotificationType::Finished,
                           [this](const ICommand::NotificationType &type,
                                  const CommandPtr &command)
                           {
                               processNotification(type, command);
                           });

        _commands.push_back(command);
        doNotifyListeners(CommandProcessor::NotificationType::Changed);

        if (isBlocked())
        {
            logError() << "CommandQueue is blocked";
        }
        else if (_commands.size() == 1U)
        {
            Async<std::shared_ptr<CommandQueue>>(
                [](const std::shared_ptr<CommandQueue> ptr)
                {
                    ptr->executeNextCommand();
                },
                shared_from_this());
        }
    }

    void CommandQueue::clear()
    {
        logMethod("CommandQueue::clear");

        for (auto &command : _commands)
        {
            Async<CommandPtr>([](const CommandPtr cmd)
                              { cmd->cancel(); }, std::move(command));
        }
        doNotifyListeners(CommandProcessor::NotificationType::Cleared);
    }

    size_t CommandQueue::clear(std::function<bool(const CommandPtr)> predicate) const
    {
        size_t count = 0;
        if (predicate)
        {
            for (const auto &command : _commands)
            {
                if (predicate(command))
                {
                    ++count;
                    Async<CommandPtr>([](const CommandPtr cmd)
                                      { cmd->cancel(); }, std::move(command));
                }
            }
        }
        else
        {
            logError() << "CommandQueue::clear: predicate is nullptr";
        }
        return count;
    }

    void CommandQueue::unblock()
    {
        logMethod("CommandQueue::unblock");

        CommandProcessor::unblock();
        Async<std::shared_ptr<CommandQueue>>(
            [](const std::shared_ptr<CommandQueue> ptr)
            {
                ptr->executeNextCommand();
            },
            shared_from_this());
    }

    void CommandQueue::processNotification(const ICommand::NotificationType &type,
                                           const CommandPtr &command)
    {
        if (type == ICommand::NotificationType::Finished)
        {
            if (!_commands.empty())
            {
                if (_commands.front() != command)
                {
                    logDebug() << "Finished command is not active, removing from queue";
                    _commands.erase(std::remove(_commands.begin(), _commands.end(), command),
                                    _commands.end());
                }
                else
                {
                    _commands.pop_front();
                    if (!_commands.empty())
                    {
                        logDebug() << "Commands remaining: " << _commands.size();
                        Async<std::shared_ptr<CommandQueue>>(
                            [](const std::shared_ptr<CommandQueue> ptr)
                            {
                                ptr->executeNextCommand();
                            },
                            shared_from_this());
                    }
                    else
                    {
                        doNotifyListeners(CommandProcessor::NotificationType::Finished);
                    }
                }
            }
            else
            {
                logError() << "CommandQueue: _commands is empty on finished notification";
            }
        }
    }

    void CommandQueue::executeNextCommand()
    {
        logMethod("CommandQueue::executeNextCommand");

        if (isBlocked())
        {
            logWarn() << "CommandQueue is blocked";
        }
        else if (!_commands.empty())
        {
            _commands.front()->execute();
        }
        else
        {
            logWarn() << "CommandQueue is empty";
        }
    }

    std::ostream &operator<<(std::ostream &out, const CommandQueue &value)
    {
        return out << value.tag();
    }

} // namespace serp
