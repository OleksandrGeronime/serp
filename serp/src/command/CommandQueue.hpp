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

#include <list>
#include <sstream>
#include <functional>

#include "command/CommandProcessor.hpp"

namespace serp
{

    class CommandQueue : public CommandProcessor
    {
    public:
        CommandQueue();
        ~CommandQueue() override;

        std::string tag() const override
        {
            std::stringstream stream;
            stream << "[CQ_" << this << "]";
            return stream.str();
        }

        void add(CommandPtr command) override;

        bool empty() const override { return _commands.empty(); }
        size_t size() const override { return _commands.size(); }

        void clear() override;

        size_t clear(std::function<bool(const CommandPtr)> predicate) const override;

        CommandPtr front() const override
        {
            return _commands.empty() ? nullptr : _commands.front();
        }

        CommandPtr back() const override
        {
            return _commands.empty() ? nullptr : _commands.back();
        }

        void unblock() override;

    private:
        void processNotification(const ICommand::NotificationType &type,
                                 const CommandPtr &command);

        std::shared_ptr<CommandQueue> shared_from_this()
        {
            return std::static_pointer_cast<CommandQueue>(
                CommandProcessor::shared_from_this());
        }

        void executeNextCommand();

        std::list<CommandPtr> _commands;
    };

    std::ostream &operator<<(std::ostream &out, const CommandQueue &value);

} // namespace serp
