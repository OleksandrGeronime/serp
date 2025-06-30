#include "LogTag.hpp"

#include <iostream>

namespace serp
{

    LogTag::LogTag(const std::string &tag)
        : _tag(tag) {}

    std::ostream &operator<<(std::ostream &os, const LogTag &tag)
    {
        return os << tag.getTag();
    }

} // namespace serp
