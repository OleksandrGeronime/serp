#include "LogStrategyCommon.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <ostream>
#include <mutex>

namespace serp
{

    static auto g_start = std::chrono::steady_clock::now();
    static std::mutex g_timeMutex;

    void dumpDateTimePrefix(std::ostream &out)
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        std::tm lt;
        {
            std::lock_guard<std::mutex> lock(g_timeMutex);
            lt = *std::localtime(&now_time);
        }

        const auto elapsedSeconds = std::chrono::duration<double>(std::chrono::steady_clock::now() - g_start);

        out << std::put_time(&lt, "%Y.%m.%d %H.%M.%S") << " "
            << elapsedSeconds.count() << " ";
    }

} // namespace serp
