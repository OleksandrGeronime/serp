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

#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <functional>

#include "core/AsyncWithDelay.hpp"
#include "core/Timer.hpp"
#include "ICommand.hpp"
#include "FSM.hpp"
#include "logger/Logger.hpp"

#define COMMAND                                    \
    static constexpr int32_t TypeId = __COUNTER__; \
    int32_t getTypeId() const override { return TypeId; }

namespace serp
{

    class Command : public ICommand,
                    public CActionsDefault,
                    public Loggable,
                    public std::enable_shared_from_this<Command>
    {
    public:
        explicit Command(std::chrono::milliseconds delay = std::chrono::milliseconds{0});

        void execute() final override;
        void cancel() final override;
        void finish(ICommand::Result result);

        uint32_t subscribe(
            ICommand::NotificationType eventType,
            std::function<void(const ICommand::NotificationType &, const std::shared_ptr<ICommand> &)> listener) override;

        void removeListener(uint32_t subscriptionId) override;

        ICommand::Result getCommandResult() const override;
        uint32_t getCmdUID() const override;

        std::string tag() const override
        {
            return "[" + getName() + "_" + std::to_string(_cmdUID) + "]";
        }

        std::shared_ptr<Command> link(const std::weak_ptr<Loggable> &parent)
        {
            Loggable::link(parent);
            return shared_from_this();
        }

    protected:
        virtual ~Command() = default;

        void onAbort() override {}
        void onCancel() override { finish(ICommand::Result::Canceled); }
        void onPostProcessing() override {}
        void onFinish() override {}
        void onWatchdog() override
        {
            logMethod("Command::onWatchdog");
            finish(ICommand::Result::Watchdog);
        }

        void wdtEnable(std::chrono::milliseconds timeout);
        void wdtReset();
        void wdtDisable();

        inline void setDelayBeforeFinish(std::chrono::milliseconds delay)
        {
            _delayBeforeFinish = delay;
        }

    private:
        void stateRunningEnter(CFSM::data_model &model) override;
        void stateFinishedEnter(CFSM::data_model &model) override;
        void stateAbortingEnter(CFSM::data_model &model) override;
        void stateCancelingEnter(CFSM::data_model &model) override;
        void statePostProcessingEnter(CFSM::data_model &model) override;
        void onReset(CFSM::data_model &model) override;

        void doNotifyListeners(ICommand::NotificationType type);

    private:
        CFSM _fsm;

        std::map<
            ICommand::NotificationType,
            std::map<uint32_t, std::function<void(const ICommand::NotificationType &, const CommandPtr &)>>>
            _listeners;

        uint32_t _nextSubscriptionId = 0;
        ICommand::Result _result = ICommand::Result::Unknown;
        TimerPtr _watchdogTimer;

        static uint32_t _nextGlobalCmdUID;
        const uint32_t _cmdUID;

        std::chrono::milliseconds _delayBeforeFinish;
        std::shared_ptr<AsyncWithDelay<std::shared_ptr<Command>>> _asyncWithDelay;
    };

    std::ostream &operator<<(std::ostream &out, const Command &value);

} // namespace serp
