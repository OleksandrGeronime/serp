#include "LogStrategyFile.hpp"

#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "serp.hpp"
#include "LogStrategyCommon.hpp"

namespace serp
{

    LogStrategyFile::LogStrategyFile(const std::filesystem::path &path,
                                     const std::string &fileNamePrefix,
                                     size_t fileSizeLimit,
                                     size_t filesCountLimit) noexcept
        : _fileSizeLimit(fileSizeLimit),
          _filesCountLimit(filesCountLimit),
          _path(path),
          _fileNamePrefix(fileNamePrefix),
          _mutex(),
          _fileStream(),
          _currentFilePath(),
          _currentFileSize(0),
          _nextFileNumber(0) {}

    LogStrategyFile::~LogStrategyFile()
    {
        std::lock_guard lock(_mutex);
        if (_fileStream.is_open())
        {
            _fileStream.close();
        }
    }

    void LogStrategyFile::init()
    {
        std::lock_guard lock(_mutex);

        if (!std::filesystem::exists(_path))
        {
            std::filesystem::create_directories(_path);
        }

        const auto configFileName = "conf_" + _fileNamePrefix + ".json";
        const auto configFilePath = _path / configFileName;

        if (std::filesystem::exists(configFilePath))
        {
            _readConfigFromJson(configFilePath);
        }

        if (_filesCountLimit > 0)
        {
            _rotateLogFile();
            _cleanupOldFiles(_path, _fileNamePrefix, _filesCountLimit);
        }
    }

    void LogStrategyFile::log(const eLogLevel &level,
                              const std::string &tag,
                              const std::string &message)
    {
        {
            std::lock_guard lock(_mutex);

            if (!_fileStream.is_open())
            {
                return;
            }

            dumpDateTimePrefix(_fileStream);
            _fileStream << level << " [" << serp::App::threadName() << "] "
                        << tag << message << '\n';
            _fileStream.flush();
        }

        static constexpr size_t formatSymbolsCount = 44U;
        size_t logSize = formatSymbolsCount + tag.size() + message.size();
        _incSizeAndRotateLogFile(logSize);
    }

    void LogStrategyFile::itcLog(const eLogLevel &level,
                                 const std::string &senderThreadName,
                                 const std::string &receiverThreadName,
                                 const std::string &classAndMethodName,
                                 const std::string &args,
                                 bool isResponse)
    {
        {
            std::lock_guard lock(_mutex);

            if (!_fileStream.is_open())
            {
                return;
            }

            dumpDateTimePrefix(_fileStream);
            _fileStream << level << " [" << senderThreadName
                        << (isResponse ? "<-" : "->") << receiverThreadName << "] "
                        << classAndMethodName << "(" << args << ")"
                        << '\n';
            _fileStream.flush();
        }

        static constexpr size_t formatSymbolsCount = 50U;
        size_t logSize = formatSymbolsCount + classAndMethodName.size() + args.size();
        _incSizeAndRotateLogFile(logSize);
    }

    void LogStrategyFile::_incSizeAndRotateLogFile(size_t logSize)
    {
        std::lock_guard lock(_mutex);

        _currentFileSize += logSize;
        if (_currentFileSize > _fileSizeLimit)
        {
            _rotateLogFile();
            _cleanupOldFiles(_path, _fileNamePrefix, _filesCountLimit);
        }
    }

    void LogStrategyFile::_rotateLogFile()
    {
        if (!_currentFilePath.empty() && _fileStream.is_open())
        {
            _fileStream.close();
        }

        auto now = std::time(nullptr);
        std::tm tm = *std::localtime(&now);

        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y%m%d_%H%M%S");
        std::string timestamp = oss.str();

        std::string fileName = _fileNamePrefix + "_" + timestamp + "_" + std::to_string(_nextFileNumber) + ".log";

        _currentFilePath = _path / fileName;
        _currentFileSize = 0;
        ++_nextFileNumber;

        _fileStream.open(_currentFilePath, std::ios::in | std::ios::out | std::ios::app);
        _fileStream.setf(std::ios::fixed, std::ios::floatfield);
    }

    void LogStrategyFile::_readConfigFromJson(const std::filesystem::path &filename)
    {
        std::ifstream file(filename);
        if (!file.is_open())
        {
            return;
        }

        std::string jsonContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();

        // crude json parsing, only for two keys
        auto parseValue = [&](const std::string &key) -> size_t
        {
            auto pos = jsonContent.find("\"" + key + "\":");
            if (pos == std::string::npos)
                return 0;

            pos = jsonContent.find(':', pos) + 1;
            auto end = jsonContent.find_first_of(",}", pos);
            if (end == std::string::npos)
                end = jsonContent.size();

            try
            {
                return std::stoul(jsonContent.substr(pos, end - pos));
            }
            catch (...)
            {
                return 0;
            }
        };

        auto fileSize = parseValue("FileSizeLimit");
        if (fileSize > 0)
        {
            _fileSizeLimit = fileSize;
        }

        auto filesCount = parseValue("FilesCountLimit");
        if (filesCount > 0)
        {
            _filesCountLimit = filesCount;
        }
    }

    void LogStrategyFile::_cleanupOldFiles(const std::filesystem::path &path,
                                           const std::string &tag,
                                           size_t maxFiles)
    {
        std::vector<std::filesystem::directory_entry> logFiles;

        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            if (entry.is_regular_file() &&
                entry.path().extension() == ".log")
            {
                const auto &filename = entry.path().filename().string();
                if (filename.compare(0, tag.size(), tag) == 0)
                {
                    logFiles.emplace_back(entry);
                }
            }
        }

        // Sort by last write time ascending (oldest first)
        std::sort(logFiles.begin(), logFiles.end(),
                  [](const auto &a, const auto &b)
                  {
                      return std::filesystem::last_write_time(a) < std::filesystem::last_write_time(b);
                  });

        if (logFiles.size() <= maxFiles)
        {
            return;
        }

        size_t filesToDelete = logFiles.size() - maxFiles;
        for (size_t i = 0; i < filesToDelete; ++i)
        {
            std::error_code ec;
            std::filesystem::remove(logFiles[i].path(), ec);
            if (ec)
            {
                std::cerr << "Failed to remove old log file " << logFiles[i].path() << ": " << ec.message() << '\n';
            }
        }
    }

} // namespace serp
