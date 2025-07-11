#pragma once

#include <set>
#include <sstream>
#include <functional>

#include "command/CommandProcessor.hpp"

namespace serp
{

    class CommandPool : public CommandProcessor
    {
    public:
        explicit CommandPool();
        ~CommandPool() override;

        std::string tag() const override
        {
            std::stringstream stream;
            stream << "[CP_" << this << "]";
            return stream.str();
        }

        void unblock() override;

        void add(CommandPtr command) override;

        bool empty() const override { return _commands.empty(); }

        size_t size() const override { return _commands.size(); }

        void clear() override;

        size_t clear(std::function<bool(const CommandPtr)> predicate) const override;

        CommandPtr front() const override
        {
            return _commands.empty() ? nullptr : *_commands.begin();
        }

        CommandPtr back() const override
        {
            return _commands.empty() ? nullptr : *std::prev(_commands.end());
        }

    private:
        void processNotification(const ICommand::NotificationType &type,
                                 const CommandPtr &command);

        // Override shared_from_this to return CommandPool pointer type
        std::shared_ptr<CommandPool> shared_from_this()
        {
            logInfo() << "shared_from_this()";
            return std::static_pointer_cast<CommandPool>(
                CommandProcessor::shared_from_this());
        }

        std::set<CommandPtr> _commands;
    };

} // namespace serp
