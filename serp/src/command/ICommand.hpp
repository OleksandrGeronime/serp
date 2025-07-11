#pragma once

#include <functional>
#include <memory>
#include <string>

namespace serp
{

    class ICommand
    {
    public:
        enum class Result : uint8_t
        {
            Unknown = 0,
            Success,
            Error,
            Canceled,
            Aborted,
            Watchdog
        };

        enum class NotificationType : uint8_t
        {
            Started = 0,
            Canceled,
            Aborted,
            Finished
        };

        virtual void execute() = 0;
        virtual void cancel() = 0;
        virtual uint32_t subscribe(
            ICommand::NotificationType eventType,
            std::function<void(const ICommand::NotificationType &,
                               const std::shared_ptr<ICommand> &)>
                listener) = 0;
        virtual void removeListener(const uint32_t subscriptionId) = 0;
        virtual void onExecute() = 0;
        virtual void onAbort() = 0;
        virtual void onCancel() = 0;
        virtual void onPostProcessing() = 0;
        virtual void onFinish() = 0;
        virtual void onWatchdog() = 0;
        virtual ICommand::Result getCommandResult() const = 0;
        virtual std::string getName() const = 0;
        virtual int32_t getTypeId() const = 0;
        virtual inline uint32_t getCmdUID() const = 0;

    protected:
        virtual ~ICommand() = default;
    };

    using CommandPtr = std::shared_ptr<ICommand>;

    template <typename T>
    std::shared_ptr<T> command_cast(CommandPtr command)
    {
        if (command && (command->getTypeId() == T::TypeId))
        {
            return std::static_pointer_cast<T>(command);
        }
        else
        {
            return std::shared_ptr<T>(nullptr);
        }
    }

    std::ostream &operator<<(std::ostream &out, const ICommand &value);
    std::ostream &operator<<(std::ostream &out, const ICommand::Result &value);
    std::ostream &operator<<(std::ostream &out,
                             const ICommand::NotificationType &value);

} // namespace serp
