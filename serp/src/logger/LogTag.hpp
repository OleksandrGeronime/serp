#pragma once

#include <ostream>
#include <string>

namespace serp
{

    class LogTag
    {
    public:
        explicit LogTag(const std::string &tag);

        const std::string &getTag() const { return _tag; }

    private:
        std::string _tag;
    };

    std::ostream &operator<<(std::ostream &os, const LogTag &tag);

} // namespace serp
