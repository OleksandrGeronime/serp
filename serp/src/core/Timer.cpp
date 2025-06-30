#include "Timer.hpp"

#include "App.hpp"
#include "EventLoop.hpp"
#include "PTimer.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    std::shared_ptr<Timer> Timer::create(
        const std::chrono::milliseconds &period,
        bool repeating,
        const std::function<void(std::shared_ptr<Timer>)> &callback)
    {
        auto timer = std::make_shared<PTimer>(period, repeating, callback);
        if (!timer)
        {
            serp::logError() << "Timer::create: failed to allocate PTimer instance";
            return nullptr;
        }

        const auto ep = App::instance().getLoopById(std::this_thread::get_id());
        if (!ep)
        {
            serp::logError() << "Timer::create: no EventLoop found for thread " << std::this_thread::get_id();
            return nullptr;
        }

        const auto id = ep->addTimer(timer);
        timer->setId(id);

        if (id == 0)
        {
            serp::logError() << "Timer::create: failed to register timer in EventLoop";
            return nullptr;
        }

        return timer;
    }

} // namespace serp
