#include "command/ICommand.hpp"

#include <sstream>
#include <type_traits>

namespace serp
{

    std::ostream &operator<<(std::ostream &out, const ICommand &value)
    {
        return out << value.getName() << "_" << value.getCmdUID();
    }

    std::ostream &operator<<(std::ostream &out, const ICommand::Result &value)
    {
        switch (value)
        {
        case ICommand::Result::Unknown:
            out << "Unknown";
            break;
        case ICommand::Result::Success:
            out << "Success";
            break;
        case ICommand::Result::Error:
            out << "Error";
            break;
        case ICommand::Result::Canceled:
            out << "Canceled";
            break;
        case ICommand::Result::Aborted:
            out << "Aborted";
            break;
        case ICommand::Result::Watchdog:
            out << "Watchdog";
            break;
        default:
            out << "ICommand::Result::UNKNOWN_TYPE("
                << static_cast<std::underlying_type_t<ICommand::Result>>(value)
                << ")";
            break;
        }
        return out;
    }

    std::ostream &operator<<(std::ostream &out, const ICommand::NotificationType &value)
    {
        switch (value)
        {
        case ICommand::NotificationType::Started:
            out << "Started";
            break;
        case ICommand::NotificationType::Canceled:
            out << "Canceled";
            break;
        case ICommand::NotificationType::Aborted:
            out << "Aborted";
            break;
        case ICommand::NotificationType::Finished:
            out << "Finished";
            break;
        default:
            out << "ICommand::NotificationType::UNKNOWN_TYPE("
                << static_cast<std::underlying_type_t<ICommand::NotificationType>>(value)
                << ")";
            break;
        }
        return out;
    }

} // namespace serp
