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

#include "EventLoop.hpp"

#include <algorithm>
#include <chrono>

#include "Call.hpp"
#include "logger/Logger.hpp"
#include "PTimer.hpp"
#include "Watchdog.hpp"

namespace serp
{

    EventLoop::EventLoop(const std::string &threadName,
                         const std::shared_ptr<Watchdog> &watchdog,
                         const std::chrono::milliseconds &watchdogTimeout)
        : _threadName(threadName),
          _stopPRequested(false),
          _thread(&EventLoop::run, this, watchdog, watchdogTimeout) {}

    EventLoop::~EventLoop()
    {
        {
            std::unique_lock lock(_mutex);
            if (!_stopPRequested)
            {
                std::queue<std::shared_ptr<Invokable>> emptyQueue;
                std::swap(_eventQueue, emptyQueue);
                _stopPRequested = true;
                _condition.notify_one();
            }
        }
        if (_thread.joinable())
        {
            _thread.join();
        }
    }

    void EventLoop::stop()
    {
        logInfo() << "EventLoop::stop " << _threadName << " (" << getThreadId() << ")";

        {
            std::unique_lock lock(_mutex);
            if (_stopPRequested)
            {
                return;
            }
            std::queue<std::shared_ptr<Invokable>> emptyQueue;
            std::swap(_eventQueue, emptyQueue);
            _stopPRequested = true;
            _condition.notify_one();
        }

        if (_thread.joinable())
        {
            _thread.join();
        }
    }

    void EventLoop::push(const std::shared_ptr<Invokable> &callable)
    {
        std::unique_lock lock(_mutex);
        if (!_stopPRequested)
        {
            _eventQueue.push(callable);
            _condition.notify_one();
        }
    }

    int32_t EventLoop::addTimer(const std::shared_ptr<PTimer> &timer)
    {
        {
            std::unique_lock lock(_mutex);
            _timers.push_back(timer);
            sortTimers();
        }
        return _nextTimerId++;
    }

    void EventLoop::removeTimer(int32_t timerId)
    {
        {
            std::unique_lock lock(_mutex);
            _timers.remove_if([timerId](const std::weak_ptr<PTimer> &weakTimer)
                              {
            auto timer = weakTimer.lock();
            return timer && timer->getId() == timerId; });
            sortTimers();
        }
    }

    void EventLoop::run(const std::shared_ptr<Watchdog> &watchdog,
                        const std::chrono::milliseconds &watchdogTimeout)
    {
        logInfo() << "EventLoop::run ENTER " << _threadName << " (" << getThreadId() << ")";

        watchdog->registerThread(getThreadId(), watchdogTimeout);

        while (true)
        {
            std::shared_ptr<Invokable> event = nullptr;
            std::chrono::milliseconds waitDuration(1000000);

            {
                std::unique_lock lock(_mutex);

                if (_stopPRequested)
                {
                    break;
                }

                if (!_timers.empty())
                {
                    auto timer = _timers.front().lock();
                    if (timer && timer->isStarted())
                    {
                        const auto now = std::chrono::steady_clock::now();
                        const auto expiry = timer->getExpirationTime();
                        if (expiry > now)
                        {
                            waitDuration = std::chrono::duration_cast<std::chrono::milliseconds>(expiry - now);
                        }
                        else
                        {
                            waitDuration = std::chrono::milliseconds(0);
                        }

                        if (waitDuration <= std::chrono::milliseconds(0))
                        {
                            event = std::make_shared<AsyncCallFunc<std::shared_ptr<PTimer>>>(timer->getAction(), std::make_tuple(timer));
                            if (timer->isRepeating())
                            {
                                timer->updateStartedTime();
                            }
                            else
                            {
                                timer->stop();
                            }
                            sortTimers();
                        }
                    }
                }

                if (!event)
                {
                    if (_eventQueue.empty() && !_stopPRequested)
                    {
                        _condition.wait_for(lock, waitDuration);
                    }

                    if (_stopPRequested)
                    {
                        break;
                    }

                    if (_eventQueue.empty())
                    {
                        continue;
                    }

                    event = _eventQueue.front();
                    _eventQueue.pop();
                }
            }

            watchdog->enterEvent(getThreadId());
            if (event)
            {
                event->invoke();
            }
            watchdog->exitEvent(getThreadId());
        }

        watchdog->unregisterThread(getThreadId());

        logInfo() << "EventLoop::run EXIT " << _threadName << " (" << getThreadId() << ")";
    }

    void EventLoop::sortTimers()
    {
        if (_timers.size() <= 1)
        {
            return;
        }

        _timers.sort([](const std::weak_ptr<PTimer> &a, const std::weak_ptr<PTimer> &b)
                     {
        auto timerA = a.lock();
        auto timerB = b.lock();

        if (!timerB) {
            return true;
        }
        if (!timerA) {
            return false;
        }

        if (!timerA->isStarted()) {
            return false;
        }
        if (!timerB->isStarted()) {
            return true;
        }
        return timerA->getExpirationTime() < timerB->getExpirationTime(); });

        _condition.notify_one();
    }

} // namespace serp
