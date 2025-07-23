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

#include "core/App.hpp"
#include "core/Method.hpp"
#include "core/Notification.hpp"
#include "core/Property.hpp"
#include "core/Service.hpp"
#include "core/SyncMethod.hpp"
#include "core/Timer.hpp"

#include "logger/Dumpers.hpp"
#include "logger/Logger.hpp"
#include "logger/strategies/LogStrategyConsole.hpp"
#include "logger/strategies/LogStrategyFile.hpp"

#include "command/Command.hpp"
#include "command/CommandProcessor.hpp"
#include "command/LambdaCommand.hpp"

#include "test_engine/TestRunner.hpp"