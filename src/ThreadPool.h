#pragma once

#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include<condition_variable>
#include<functional>
#include<memory>
#include<mutex>
#include<queue>
#include<thread>
#include <utility>
#include<vector>

using namespace std;

class ThreadPool
{
private:
    vector<thread> m_threads;
    queue<function<void()>> m_tasks;
    mutex m_mutex;
    condition_variable m_cv;
    bool m_stop;

    static shared_ptr<ThreadPool> m_inctance;

private:
    ThreadPool(int numThread);

public:
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool operator=(const ThreadPool& other) = delete;
    ~ThreadPool();

    template<class F, class ...Args>
    void Enqueue(F &&f, Args&&(...args));

    static shared_ptr<ThreadPool> GetInstance();

};

template<class F, class... Args>
inline void ThreadPool::Enqueue(F&& f, Args&&... args)
{
    function<void()> task = bind(forward<F>(f), forward<Args>(args)...);
    {
        lock_guard<mutex> lock(m_mutex);
        m_tasks.emplace(move(task));
    }
    m_cv.notify_one();
}

#endif // !_THREAD_POOL_H_