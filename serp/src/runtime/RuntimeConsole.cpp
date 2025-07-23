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

#include "RuntimeConsole.hpp"

namespace serp
{

    void RuntimeConsole::run(bool runInBackground)
    {
        auto runLoop = [this]()
        {
            std::string input;
            while (!_stop)
            {
                std::cout << "> ";

                std::string line;
                if (!std::getline(std::cin, line))
                {
                    std::cout << "[ERROR] Unable to read input. Exiting...\n";
                    break;
                }

                size_t pos = line.find("(");
                if (pos == std::string::npos)
                {
                    std::cout << "[WARN] Invalid command format. Expected: command(arg1,arg2,...)\n";
                    continue;
                }

                std::string command = line.substr(0, pos);
                std::string args_str = line.substr(pos + 1, line.size() - pos - 2);

                std::vector<std::string> args;
                std::istringstream arg_stream(args_str);
                std::string arg;
                while (std::getline(arg_stream, arg, ','))
                {
                    args.push_back(arg);
                }

                auto it = serp::Runtime::sMethodMap.find(command);
                if (it != serp::Runtime::sMethodMap.end())
                {
                    auto response = it->second(args);
                    std::this_thread::sleep_for(std::chrono::microseconds(100));
                    std::cout << "[OK] Response: " << response << std::endl;
                }
                else
                {
                    std::cout << "[FAIL] Unknown command: '" << command << std::endl;
                }
            }
        };

        if (runInBackground)
        {
            std::thread(runLoop).detach();
        }
        else
        {
            runLoop();
        }
    }

} // namespace serp