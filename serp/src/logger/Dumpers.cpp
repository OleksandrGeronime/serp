#include "Dumpers.hpp"

namespace serp
{

    std::ostream &operator<<(std::ostream &out, const std::chrono::milliseconds &value)
    {
        return out << value.count() << "ms";
    }

    std::ostream &operator<<(std::ostream &out, const std::chrono::seconds &value)
    {
        return out << value.count() << "s";
    }

    std::ostream &operator<<(std::ostream &out, const std::runtime_error &value)
    {
        return out << "error(" << value.what() << ")";
    }

    std::ostream &operator<<(std::ostream &out, const log_bool &value)
    {
        return out << (value._value ? "true" : "false");
    }

} // namespace serp
