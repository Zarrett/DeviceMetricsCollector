#pragma once

#include <boost/asio.hpp>

namespace MetricsCollector {

    using TaskGuard = std::weak_ptr<bool>;
    using Timer = boost::asio::steady_timer;
    using Handle = std::weak_ptr<Timer>;
    using IOContext = boost::asio::io_context;
    using TimeDuration = std::chrono::steady_clock::duration;
    using TaskCallback = std::function<void()>;


    class Scheduler {
    public:
        explicit Scheduler(IOContext& context) noexcept;
        Scheduler(const Scheduler& other) = delete;
        Scheduler(Scheduler&& other) noexcept;

        Scheduler& operator=(const Scheduler& other) = delete;
        Scheduler& operator=(Scheduler&& other) noexcept;

        virtual ~Scheduler() noexcept;

        template<class T>
        Handle runOnce(
            const TaskGuard& guard,
            T&& taskCallback,
            const TimeDuration& delay,
            const std::string& taskName = "");

        template<class T>
        Handle runOnce(const TaskGuard& guard, T&& taskCallback, const std::string& taskName = "");

        void stop(const Handle& handle) noexcept;

        void stopAll() noexcept;

    private:
        Handle runOnceInternal(
            const TaskGuard& guard,
            const TaskCallback& taskCallback,
            const TimeDuration& delay,
            const std::string& taskName = "");
        Handle runOnceInternal(
            const TaskGuard& guard, const TaskCallback& taskCallback, const std::string& taskName = "");

        struct ImplT;
        std::unique_ptr<ImplT> m_impl{ nullptr };
    };

    template<class T>
    Handle Scheduler::runOnce(
        const TaskGuard& guard,
        T&& taskCallback,
        const TimeDuration& delay,
        const std::string& taskName)
    {
        return runOnceInternal(
            std::move(guard), makeSharedFunction(std::move(taskCallback)), delay, taskName);
    }

    template<class T>
    Handle Scheduler::runOnce(
        const TaskGuard& guard, T&& taskCallback, const std::string& taskName)
    {
        return runOnceInternal(std::move(guard), makeSharedFunction(std::move(taskCallback)), taskName);
    }

} // namespace MetricsCollector
