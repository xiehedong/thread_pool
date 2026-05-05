#include "ThreadPool.h"
#include <functional>
#include <memory>
#include <mutex>
#include <utility>


shared_ptr<ThreadPool> ThreadPool::m_inctance = nullptr;

ThreadPool::ThreadPool(int numThread): m_stop(false)
{
    for (int i = 0; i < numThread; i++)
    {
        m_threads.emplace_back([this]() {
            while (true)
            {
                unique_lock<mutex> lock(m_mutex);
                m_cv.wait(lock, [this]() {
                    return !m_tasks.empty() || m_stop;
                    });

                if (m_tasks.empty() && m_stop)
                    return;

                function<void()> task(move(m_tasks.front()));
                m_tasks.pop();
                lock.unlock();

                task();
            }
        });

    }
}

ThreadPool::~ThreadPool()
{
    {
        lock_guard<mutex> lock(m_mutex);
        m_stop = true;
    }

    m_cv.notify_all();

    for (auto& thread : m_threads)
    {
        thread.join();
    }
}

shared_ptr<ThreadPool> ThreadPool::GetInstance()
{
    static once_flag flag;
    call_once(flag, []() {
        m_inctance = shared_ptr<ThreadPool>(new ThreadPool(4));
        });
    return m_inctance;
}
