#include "ReplyHolder.hpp"

namespace serp
{

    void ReplyHolder::clear()
    {
        if (!_repliesQueue.empty())
        {
            logWarn() << "mRepliesQueue still contains " << _repliesQueue.size() << " replies";
            std::queue<std::any> empty;
            std::swap(_repliesQueue, empty);
        }

        if (!_replyFuncsQueue.empty())
        {
            logWarn() << "mReplyFuncsQueue still contains " << _replyFuncsQueue.size() << " replyFuncs";
            std::queue<std::any> empty;
            std::swap(_replyFuncsQueue, empty);
        }
    }

} // namespace serp
