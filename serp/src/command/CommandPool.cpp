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

#include "CommandPool.hpp"

namespace serp
{

    CommandPool::CommandPool() : _commands{}
    {
        logMethod("CommandPool::CommandPool");
    }

    CommandPool::~CommandPool()
    {
        logMethod("CommandPool::~CommandPool");
    }

    void CommandPool::unblock()
    {
        logMethod("CommandPool::unblock");

        CommandProcessor::unblock();

        // Execute all commands asynchronously
        for (auto &command : _commands)
        {
            Async<CommandPtr>([](const CommandPtr &cmd)
                              { cmd->execute(); }, command);
        }
    }

    void CommandPool::add(CommandPtr command)
    {
        logMethod("CommandPool::add", command);

        if (_clearSubscriptionId != 0U)
        {
            logWarn() << "Pool clearing is in progress, cancelling added command";
            command->cancel();
            return;
        }

        // Subscribe to finished notification
        command->subscribe(ICommand::NotificationType::Finished,
                           [this](const ICommand::NotificationType &type, const CommandPtr &cmd)
                           {
                               processNotification(type, cmd);
                           });

        _commands.insert(std::move(command));

        doNotifyListeners(CommandProcessor::NotificationType::Changed);

        if (isBlocked())
        {
            logError() << "CommandPool is blocked, command execution postponed";
        }
        else
        {
            // Execute command immediately
            (*_commands.rbegin())->execute(); // last inserted command
        }
    }

    void CommandPool::clear()
    {
        logMethod("CommandPool::clear");

        for (auto &command : _commands)
        {
            Async<CommandPtr>([](const CommandPtr &cmd)
                              { cmd->execute(); }, command);
        }

        doNotifyListeners(CommandProcessor::NotificationType::Cleared);
    }

    size_t CommandPool::clear(std::function<bool(const CommandPtr)> predicate) const
    {
        if (!predicate)
        {
            logError() << "CommandPool::clear: predicate is nullptr";
            return 0;
        }

        size_t count = 0;
        for (auto &command : _commands)
        {
            if (predicate(command))
            {
                ++count;
                Async<CommandPtr>([](const CommandPtr &cmd)
                                  { cmd->execute(); }, command);
            }
        }
        return count;
    }

    void CommandPool::processNotification(const ICommand::NotificationType &type,
                                          const CommandPtr &command)
    {
        if (type == ICommand::NotificationType::Finished)
        {
            if (!_commands.empty())
            {
                _commands.erase(command);

                if (_commands.empty())
                {
                    doNotifyListeners(CommandProcessor::NotificationType::Finished);
                }
            }
            else
            {
                logError() << "CommandPool::processNotification: _commands is empty";
            }
        }
    }

} // namespace serp
