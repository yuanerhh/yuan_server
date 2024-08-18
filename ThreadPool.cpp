#include "ThreadPool.h"

using namespace std;

namespace yuan {

CThreadPool::CThreadPool(size_t nThreadNum) 
    : m_nThreadNum(nThreadNum), m_isRunning(true)
{
    for (int i = 0; i < nThreadNum; ++i)
    {
        m_lstThreads.emplace_back(thread(&CThreadPool::__ThreadProc, this));
    }
}

CThreadPool::~CThreadPool()
{
    m_isRunning = false;
    for(auto& th : m_lstThreads)
    {
        if (th.joinable())
        {
            th.join();
        }
    }

    m_lstThreads.clear();
}

void CThreadPool::AddTask(const TASK& task)
{
    lock_guard<mutex> lk(m_lockTaskList);
    m_lstTaskLists.emplace_back(task);
}

void CThreadPool::__ThreadProc()
{
    while (m_isRunning)
    {
        unique_lock<std::mutex> lk(m_lockTaskList);
        while (m_lstTaskLists.size() <= 0)
        {
            m_condTaskList.wait_for(lk, chrono::milliseconds(1000));
        }
        
        auto task = m_lstTaskLists.back();
        m_lstTaskLists.pop_back();
        lk.unlock();

        task();
    }
    
}

}