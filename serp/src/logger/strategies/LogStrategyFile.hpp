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

#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>

#include "ILogStrategy.hpp"

namespace serp
{

    class LogStrategyFile : public ILogStrategy
    {
    public:
        LogStrategyFile() = delete;

        LogStrategyFile(const std::filesystem::path &path,
                        const std::string &fileNamePrefix,
                        size_t fileSizeLimit,
                        size_t filesCountLimit) noexcept;

        ~LogStrategyFile() override;

        void init() override;

        void log(const eLogLevel &level,
                 const std::string &tag,
                 const std::string &message) override;

        void itcLog(const eLogLevel &level,
                    const std::string &senderThreadName,
                    const std::string &receiverThreadName,
                    const std::string &classAndMethodName,
                    const std::string &args,
                    bool isResponse) override;

    private:
        void _incSizeAndRotateLogFile(size_t logSize);
        void _rotateLogFile();
        void _readConfigFromJson(const std::filesystem::path &filename);
        static void _cleanupOldFiles(const std::filesystem::path &path,
                                     const std::string &tag,
                                     size_t maxFiles);

        size_t _fileSizeLimit;
        size_t _filesCountLimit;
        const std::filesystem::path _path;
        const std::string _fileNamePrefix;

        std::mutex _mutex;
        std::fstream _fileStream;
        std::filesystem::path _currentFilePath;
        size_t _currentFileSize{0};
        size_t _nextFileNumber{0};
    };

} // namespace serp
