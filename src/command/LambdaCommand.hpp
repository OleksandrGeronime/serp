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

#include <functional>
#include <memory>
#include <string>

#include "Command.hpp"

namespace serp
{

    class LambdaCommand final : public Command
    {
    public:
        static constexpr int32_t TypeId = -1;
        int32_t getTypeId() const override { return TypeId; }

        LambdaCommand(const std::string &name,
                      const std::function<void(std::shared_ptr<LambdaCommand>)> &onExecuteFunc);
        ~LambdaCommand() override;

        void onExecute() override;
        void onCancel() override;
        void onAbort() override;
        void onPostProcessing() override;

        std::string getName() const override { return mName; }

        void setCancelProcessor(const std::function<void()> &onCancelFunc)
        {
            mOnCancelFunc = onCancelFunc;
        }

        void setAbortProcessor(const std::function<void()> &onAbortFunc)
        {
            mOnAbortFunc = onAbortFunc;
        }

        void setPostProcessingProcessor(
            const std::function<void(const ICommand::Result &)> &onPostProcessingFunc)
        {
            mOnPostProcessingFunc = onPostProcessingFunc;
        }

    private:
        std::shared_ptr<LambdaCommand> shared_from_this()
        {
            return std::static_pointer_cast<LambdaCommand>(Command::shared_from_this());
        }

        const std::string mName;
        std::function<void(const std::shared_ptr<LambdaCommand> &)> mOnExecuteFunc;
        std::function<void()> mOnCancelFunc;
        std::function<void()> mOnAbortFunc;
        std::function<void(const ICommand::Result &)> mOnPostProcessingFunc;
    };

    using LambdaCommandPtr = std::shared_ptr<LambdaCommand>;

} // namespace serp
