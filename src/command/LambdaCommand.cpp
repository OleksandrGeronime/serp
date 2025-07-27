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

#include "command/LambdaCommand.hpp"

namespace serp
{

    LambdaCommand::LambdaCommand(
        const std::string &name,
        const std::function<void(LambdaCommandPtr)> &onExecuteFunc)
        : Command(),
          mName{name},
          mOnExecuteFunc{onExecuteFunc},
          mOnCancelFunc{nullptr},
          mOnAbortFunc{nullptr},
          mOnPostProcessingFunc{nullptr}
    {
        logMethod("LambdaCommand::LambdaCommand", name);
    }

    LambdaCommand::~LambdaCommand()
    {
        logMethod("LambdaCommand::~LambdaCommand");
    }

    void LambdaCommand::onExecute()
    {
        logMethod("LambdaCommand::onExecute");
        if (mOnExecuteFunc)
        {
            mOnExecuteFunc(shared_from_this());
        }
        else
        {
            logError() << "mOnExecuteFunc is null";
            finish(ICommand::Result::Error);
        }
    }

    void LambdaCommand::onCancel()
    {
        logMethod("LambdaCommand::onCancel");
        if (mOnCancelFunc)
        {
            mOnCancelFunc();
        }
        else
        {
            logDebug() << "mOnCancelFunc is null, calling base Command::onCancel()";
        }
        Command::onCancel();
    }

    void LambdaCommand::onAbort()
    {
        logMethod("LambdaCommand::onAbort");
        if (mOnAbortFunc)
        {
            mOnAbortFunc();
        }
        else
        {
            logDebug() << "mOnAbortFunc is null, calling base Command::onAbort()";
        }
        Command::onAbort();
    }

    void LambdaCommand::onPostProcessing()
    {
        logMethod("LambdaCommand::onPostProcessing");
        if (mOnPostProcessingFunc)
        {
            mOnPostProcessingFunc(getCommandResult());
        }
        else
        {
            logDebug() << "mOnPostProcessingFunc is null, calling base Command::onPostProcessing()";
        }
        Command::onPostProcessing();
    }

} // namespace serp
