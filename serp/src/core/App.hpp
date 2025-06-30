#pragma once

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>

#include "Invokable.hpp"

namespace serp
{

    class Watchdog;
    class EventLoop;

    class App final
    {
    public:
        class WatchdogException : public std::exception
        {
        public:
            explicit WatchdogException(const std::thread::id &id);
            const char *what() const noexcept override;

        private:
            std::string _message;
        };

        App(const App &) = delete;
        App &operator=(const App &) = delete;
        App(App &&) = delete;
        App &operator=(App &&) = delete;
        ~App();

        static App &instance();

        static const std::string UNKNOWN_THREAD_NAME;

        static void run();
        static void exit();

        static void createEventLoop(
            const std::string &threadName,
            const std::chrono::milliseconds &timeout = std::chrono::milliseconds(1000));

        static void stopEventLoop(const std::string &threadName);

        static const std::string &currentThreadName();

        static void invoke(const std::string &threadName,
                           const std::shared_ptr<Invokable> &callable);

        std::shared_ptr<EventLoop> processorByName(const std::string &name) const;
        std::shared_ptr<EventLoop> processorById(const std::thread::id &id) const;

        std::shared_ptr<Watchdog> watchdog() const { return _watchdog; }

    private:
        App();

        void registerProcessor(const std::shared_ptr<EventLoop> &processor);
        void unregisterProcessor(const std::thread::id &id);

        mutable std::recursive_mutex _lock;
        std::map<std::thread::id, std::shared_ptr<EventLoop>> _processors;
        std::shared_ptr<Watchdog> _watchdog;
    };

} // namespace serp
