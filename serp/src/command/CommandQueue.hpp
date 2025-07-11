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
