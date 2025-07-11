#include "command/Command.hpp"

#include <set>
#include <sstream>

#include "logger/Logger.hpp"

namespace serp
{

    uint32_t Command::_nextGlobalCmdUID = 1U;

    Command::Command(const std::chrono::milliseconds delay)
        : _fsm{this},
          _listeners{},
          _nextSubscriptionId{0U},
          _result{ICommand::Result::Unknown},
          _watchdogTimer{},
          _cmdUID{_nextGlobalCmdUID},
          _delayBeforeFinish{delay},
          _asyncWithDelay{nullptr}
    {
        _nextGlobalCmdUID++;
        _fsm.init();
    }

    void Command::execute()
    {
        Async<std::shared_ptr<Command>>(
            [](const std::shared_ptr<Command> &ptr)
            {
                ptr->_fsm.dispatch(&CFSM::state::trigger_RUN);
            },
            shared_from_this());
    }

    uint32_t Command::getCmdUID() const { return _cmdUID; }

    void Command::cancel()
    {
        logMethod("Command::cancel");

        Async<std::shared_ptr<Command>>(
            [](const std::shared_ptr<Command> &ptr)
            {
                ptr->_fsm.dispatch(&CFSM::state::trigger_CANCEL);
            },
            shared_from_this());
    }

    void Command::finish(ICommand::Result result)
    {
        logMethod("Command::finish", result);

        _result = result;
        const auto finishLambda = [](const std::shared_ptr<Command> &ptr)
        {
            ptr->_fsm.dispatch(&CFSM::state::trigger_FINISH);
        };

        if (_delayBeforeFinish == std::chrono::milliseconds{0U})
        {
            Async<std::shared_ptr<Command>>(finishLambda, shared_from_this());
        }
        else
        {
            if (!_asyncWithDelay)
            {
                logInfo() << "finish will be triggered in " << _delayBeforeFinish << " milliseconds";
                _asyncWithDelay = std::make_shared<AsyncWithDelay<std::shared_ptr<Command>>>(
                    _delayBeforeFinish, finishLambda, shared_from_this());
            }
            else
            {
                logWarn() << "_asyncWithDelay is already initialized";
            }
        }
    }

    uint32_t Command::subscribe(
        ICommand::NotificationType eventType,
        std::function<void(const ICommand::NotificationType &, const CommandPtr &)> listener)
    {
        ++_nextSubscriptionId;
        _listeners[eventType][_nextSubscriptionId] = std::move(listener);
        return _nextSubscriptionId;
    }

    void Command::removeListener(uint32_t subscriptionId)
    {
        std::set<ICommand::NotificationType> typesToRemove;

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

    ICommand::Result Command::getCommandResult() const { return _result; }

    void Command::wdtEnable(std::chrono::milliseconds timeout)
    {
        logMethod("Command::wdtEnable", timeout);

        _watchdogTimer = Timer::create(timeout, false, [this](const TimerPtr &)
                                       {
        logInfo() << "Watchdog calling onWatchdog";
        onWatchdog(); });

        if (_watchdogTimer)
        {
            _watchdogTimer->start();
        }
        else
        {
            logError() << "Command::wdtEnable: failed to create timer.";
        }
    }

    void Command::wdtReset()
    {
        logMethod("Command::wdtReset");

        if (_watchdogTimer)
        {
            onReset(_fsm._model);
            _watchdogTimer->restart();
        }
        else
        {
            logWarn() << "Watchdog timer for " << shared_from_this() << " is not defined";
        }
    }

    void Command::wdtDisable()
    {
        logMethod("Command::wdtDisable");

        if (_watchdogTimer)
        {
            _watchdogTimer->stop();
        }
        else
        {
            logWarn() << "Watchdog timer for " << shared_from_this() << " is not defined";
        }
    }

    void Command::stateRunningEnter(CFSM::data_model &)
    {
        doNotifyListeners(ICommand::NotificationType::Started);
        onExecute();
    }

    void Command::stateFinishedEnter(CFSM::data_model &)
    {
        onFinish();
        doNotifyListeners(ICommand::NotificationType::Finished);
    }

    void Command::stateAbortingEnter(CFSM::data_model &)
    {
        doNotifyListeners(ICommand::NotificationType::Started);
        onAbort();
    }

    void Command::stateCancelingEnter(CFSM::data_model &)
    {
        onCancel();
    }

    void Command::statePostProcessingEnter(CFSM::data_model &)
    {
        onPostProcessing();

        Async<std::shared_ptr<Command>>(
            [](const std::shared_ptr<Command> &ptr)
            {
                ptr->_fsm.dispatch(&CFSM::state::trigger_FINISHED);
            },
            shared_from_this());
    }

    void Command::onReset(CFSM::data_model &) {}

    void Command::doNotifyListeners(const ICommand::NotificationType type)
    {
        if (_listeners.count(type) == 0)
            return;

        for (const auto &[subscriptionId, listener] : _listeners[type])
        {
            Async<
                std::function<void(const ICommand::NotificationType &, const CommandPtr &)>,
                CommandPtr, ICommand::NotificationType>(
                [](const std::function<void(const ICommand::NotificationType &, const CommandPtr &)> &listener,
                   const CommandPtr &command,
                   const ICommand::NotificationType type)
                {
                    listener(type, command);
                },
                listener, shared_from_this(), type);
        }
    }

    std::ostream &operator<<(std::ostream &out, const Command &value)
    {
        return out << value.getName() << "(" << value.getCmdUID() << ")";
    }

} // namespace serp
