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
