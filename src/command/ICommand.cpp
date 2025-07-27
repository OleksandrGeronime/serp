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
