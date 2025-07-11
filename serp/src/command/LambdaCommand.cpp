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
