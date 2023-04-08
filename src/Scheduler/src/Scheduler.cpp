#include "Scheduler.hpp"

#include <memory>
#include <mutex>
#include <iostream>
#include <boost/asio/error.hpp>

namespace MetricsCollector {
using TimerSharedPtr = std::shared_ptr<Timer>;
using TimerIterator = std::vector<TimerSharedPtr>::iterator;
using ErrorCode = boost::system::error_code;

class Task {
public:
    Task(
        const TaskGuard& guard,
        const std::function<void()>& taskCallback,
        const std::string& message)
        : m_message(message)
        , m_taskCallback(taskCallback)
        , m_guard(guard) {};

    void execute()
    {
        try {
            if (auto lockedGuard = m_guard.lock()) {
                if (*lockedGuard) {
                    m_taskCallback();
                }
                else {
                    std::cout << "The task guard is disabled." << std::endl;
                }
            }
            else {
                std::cout << "The owner of task was expired." << std::endl;
            }
        }
        catch (const std::exception& ex) {
            std::cout << "Task \"" << m_message << "\" finished with error: " << ex.what() << std::endl;
        }
        catch (...) {
            std::cout << "Unhandled exception" << std::endl;
        }
    }

    const std::string& name() const
    {
        return m_message;
    }

private:
    std::string m_message;
    std::function<void()> m_taskCallback;
    TaskGuard m_guard;
};

struct Scheduler::ImplT {
    explicit ImplT(IOContext& context) noexcept;

    decltype(auto) find(TimerSharedPtr timer) noexcept;

    void stop(TimerIterator iterator) noexcept;

    static void logError(const ErrorCode& error) noexcept;
    static void timerCancel(const TimerSharedPtr& timer) noexcept;

    void setCurrentTask(const std::string& name);
    std::string currentTask();

    IOContext& m_ctx;
    std::vector<TimerSharedPtr> m_timers;
    std::recursive_mutex m_timerMutex;
    std::mutex m_taskNameMtx;
    std::string m_currentTaskName;
};

Scheduler::ImplT::ImplT(IOContext& context) noexcept
    : m_ctx(context)
{
}

decltype(auto) Scheduler::ImplT::find(TimerSharedPtr timer) noexcept
{
    const std::lock_guard<std::recursive_mutex> lock(m_timerMutex);

    return std::find_if(
        m_timers.begin(),
        m_timers.end(),
        [timer = std::move(timer)](const auto& element) { return (timer == element); });
}


void Scheduler::ImplT::stop(const TimerIterator iterator) noexcept
{
    if ((iterator != m_timers.end())) {
        timerCancel(*iterator);
    }
    else {
        std::cout << "The task has not found." << std::endl;
    }
}

void Scheduler::ImplT::logError(const ErrorCode& error) noexcept
{
    if (error) {
        if (error == boost::asio::error::operation_aborted) {
            std::cout << "The task was stopped" << std::endl;
        }
        else {
            std::cout << "The task execution error: " << error.message() << std::endl;
        }
    }
}

void Scheduler::ImplT::timerCancel(const TimerSharedPtr& timer) noexcept
{
    ErrorCode cancelError;

    timer->cancel(cancelError);

    if (cancelError) {
        std::cout << "The task canceling is incorrect: " << cancelError.message() << std::endl;
    }
    else {
        std::cout << "The task has got a cancel signal." << std::endl;
    }
}

void Scheduler::ImplT::setCurrentTask(const std::string& name)
{
    const std::lock_guard<std::mutex> nameLock(m_taskNameMtx);
    m_currentTaskName = name;
}

std::string Scheduler::ImplT::currentTask()
{
    const std::lock_guard<std::mutex> nameLock(m_taskNameMtx);
    return m_currentTaskName;
}

Scheduler::Scheduler(IOContext& context) noexcept
    : m_impl{ std::make_unique<ImplT>(context) }
{
}

Scheduler::Scheduler(Scheduler&& other) noexcept
    : m_impl(std::move(other.m_impl))
{
}

Scheduler& Scheduler::operator=(Scheduler&& other) noexcept
{
    if (this == &other) {
        return *this;
    }
    m_impl = std::move(other.m_impl);

    return *this;
}

Scheduler::~Scheduler() noexcept
{
    stopAll();
}

Handle Scheduler::runOnceInternal(
    const TaskGuard& guard,
    const TaskCallback& taskCallback,
    const TimeDuration& delay,
    const std::string& taskName)
{
    const auto delayTimer = std::make_shared<Timer>(m_impl->m_ctx);

    {
        const std::lock_guard<std::recursive_mutex> lock(m_impl->m_timerMutex);
        m_impl->m_timers.emplace_back(delayTimer);
    }

    delayTimer->expires_from_now(delay);

    delayTimer->async_wait([=, task = std::make_unique<Task>(guard, taskCallback, taskName)](
        const ErrorCode& error) mutable {
            if (error) {
                m_impl->logError(error);
            }
            else {
                m_impl->setCurrentTask(task->name());
                task->execute();
            }

            {
                const std::lock_guard<std::recursive_mutex> lock(m_impl->m_timerMutex);
                m_impl->m_timers.erase(m_impl->find(delayTimer));
            }
        });

    return delayTimer;
}

Handle Scheduler::runOnceInternal(
    const TaskGuard& guard, const TaskCallback& taskCallback, const std::string& taskName)
{
    return runOnceInternal(guard, taskCallback, std::chrono::milliseconds(0), taskName);
}

void Scheduler::stop(const Handle& handle) noexcept
{
    const auto iterator = m_impl->find(handle.lock());

    m_impl->stop(iterator);
}

void Scheduler::stopAll() noexcept
{
    const std::lock_guard<std::recursive_mutex> lock(m_impl->m_timerMutex);

    std::for_each(m_impl->m_timers.begin(), m_impl->m_timers.end(), [this](const auto& element) {
        stop(element);
        });
}

} // namespace MetricsCollector