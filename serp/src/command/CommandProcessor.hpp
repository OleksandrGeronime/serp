#pragma once

#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <functional>

#include "ICommand.hpp"
#include "LambdaCommand.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    class CommandProcessor : public Loggable,
                             public std::enable_shared_from_this<CommandProcessor>
    {
    public:
        enum class Type : uint8_t
        {
            Queue = 0,
            Pool = 1
        };

        enum class NotificationType : uint8_t
        {
            Changed = 0,
            Cleared = 1,
            Finished = 2
        };

        static std::shared_ptr<CommandProcessor> create(Type type);

        CommandProcessor();
        virtual ~CommandProcessor() = default;

        template <typename TCommand>
        std::shared_ptr<TCommand> add()
        {
            auto command = std::make_shared<TCommand>();
            add(command);
            return command;
        }

        template <typename TCommand, typename... Args>
        std::shared_ptr<TCommand> add(Args &&...args)
        {
            auto command = std::make_shared<TCommand>(std::forward<Args>(args)...);
            add(command);
            return command;
        }

        std::shared_ptr<LambdaCommand> add(
            const std::string &name,
            std::function<void(std::shared_ptr<LambdaCommand>)> processor,
            const std::shared_ptr<Loggable> parent = nullptr)
        {
            auto inlineCommand = std::make_shared<LambdaCommand>(name, std::move(processor));
            add(inlineCommand);
            if (parent)
            {
                inlineCommand->link(parent);
            }
            return inlineCommand;
        }

        virtual bool empty() const = 0;

        virtual size_t size() const = 0;

        virtual CommandPtr front() const = 0;

        virtual CommandPtr back() const = 0;

        virtual void clear() = 0;

        void clear(std::function<void(void)> callback);

        virtual size_t clear(std::function<bool(const CommandPtr)> predicate) const = 0;

        size_t clear(std::set<int32_t> types) const;

        virtual void block();

        virtual void unblock();

        bool isBlocked() const { return _blocked; }

        virtual uint32_t subscribe(
            NotificationType notificationType,
            std::function<void(const NotificationType &notificationType,
                               const std::shared_ptr<CommandProcessor> &commandProcessor)>
                listener);

        void unsubscribe(const uint32_t &subscriptionId);

    protected:
        void doNotifyListeners(NotificationType type);
        virtual void add(CommandPtr command) = 0;

        uint32_t _clearSubscriptionId{0};

    private:
        std::map<NotificationType,
                 std::map<uint32_t, std::function<void(
                                        const NotificationType &notificationType,
                                        const std::shared_ptr<CommandProcessor> &commandProcessor)>>>
            _listeners;

        uint32_t _nextSubscriptionId{0};
        bool _blocked{false};
    };

    using CommandProcessorPtr = std::shared_ptr<CommandProcessor>;

    std::ostream &operator<<(std::ostream &out, const CommandProcessor::Type &value);
    std::ostream &operator<<(std::ostream &out, const CommandProcessor::NotificationType &value);

} // namespace serp
