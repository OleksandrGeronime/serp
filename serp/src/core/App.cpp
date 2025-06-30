#include "App.hpp"

#include <algorithm>
#include <sstream>

#include "EventLoop.hpp"
#include "Watchdog.hpp"
#include "logger/Logger.hpp"

namespace serp
{

    const std::string App::UNKNOWN_THREAD_NAME = "UNKNOWN";

    App::WatchdogException::WatchdogException(const std::thread::id &threadId)
        : _message{}
    {
        const auto ep = App::instance().processorById(threadId);
        const auto &name = ep ? ep->getThreadName() : UNKNOWN_THREAD_NAME;

        std::ostringstream oss;
        oss << "Thread " << name << " (" << threadId << ") timeout";
        _message = oss.str();
    }

    const char *App::WatchdogException::what() const noexcept
    {
        return _message.c_str();
    }

    App::App()
        : _lock{},
          _processors{},
          _watchdog{std::make_shared<Watchdog>()} {}

    App::~App()
    {
        std::unique_lock lock(_lock);
        _processors.clear();
    }

    App &App::instance()
    {
        static App app;
        return app;
    }

    void App::run()
    {
        instance()._watchdog->start();
    }

    void App::exit()
    {
        instance()._watchdog->stop();
    }

    void App::createEventLoop(const std::string &threadName,
                              const std::chrono::milliseconds &timeout)
    {
        serp::logMethod("App::createEventLoop", threadName);

        const auto ep = std::make_shared<EventLoop>(
            threadName, instance()._watchdog, timeout);

        instance().registerProcessor(ep);
    }

    void App::stopEventLoop(const std::string &threadName)
    {
        serp::logMethod("App::stopEventLoop", threadName);

        const auto ep = instance().processorByName(threadName);
        if (!ep)
        {
            logError() << "Unknown thread: " << threadName;
            return;
        }

        instance().unregisterProcessor(ep->getThreadId());
        ep->stop();
    }

    const std::string &App::currentThreadName()
    {
        const auto ep = instance().processorById(std::this_thread::get_id());
        return ep ? ep->getThreadName() : UNKNOWN_THREAD_NAME;
    }

    void App::invoke(const std::string &threadName,
                     const std::shared_ptr<Invokable> &callable)
    {
        const auto ep = instance().processorByName(threadName);
        if (!ep)
        {
            logError() << "invoke() failed. Unknown thread name: " << threadName;
            return;
        }

        ep->push(callable);
    }

    std::shared_ptr<EventLoop> App::processorById(const std::thread::id &id) const
    {
        std::unique_lock lock(_lock);
        const auto it = _processors.find(id);
        return (it != _processors.end()) ? it->second : nullptr;
    }

    std::shared_ptr<EventLoop> App::processorByName(const std::string &name) const
    {
        std::unique_lock lock(_lock);
        const auto it = std::find_if(
            _processors.begin(), _processors.end(),
            [&name](const auto &entry)
            {
                return entry.second->getThreadName() == name;
            });

        return (it != _processors.end()) ? it->second : nullptr;
    }

    void App::registerProcessor(const std::shared_ptr<EventLoop> &processor)
    {
        std::thread::id id;
        std::string name;
        size_t beforeSize = 0;
        bool inserted = false;

        {
            std::unique_lock lock(_lock);
            id = processor->getThreadId();
            name = processor->getThreadName();
            beforeSize = _processors.size();

            if (_processors.count(id) == 0)
            {
                _processors[id] = processor;
                inserted = true;
            }
        }

        if (inserted)
        {
            serp::logInfo() << "Registered processor: " << name << " (id=" << id << "), total=" << beforeSize + 1;
        }
        else
        {
            serp::logWarn() << "Processor already registered: " << name << " (id=" << id << ")";
        }
    }

    void App::unregisterProcessor(const std::thread::id &id)
    {
        std::unique_lock lock(_lock);
        _processors.erase(id);
    }

} // namespace serp
