#include <thread>
#include <functional>
#include <list>
#include <mutex>
#include <condition_variable>
#include <atomic>

namespace yuan {

class CThreadPool
{
public:
    using TASK = std::function<void(void)>;
    using TASK_QUEUE = std::list<TASK>;

    CThreadPool(size_t nThreadNum = 5);
    ~CThreadPool();

    void AddTask(const TASK& task);

private:
    void __ThreadProc();

private:
    size_t m_nThreadNum = 0;
    std::list<std::thread> m_lstThreads;
    std::atomic_bool m_isRunning;
    
    TASK_QUEUE m_lstTaskLists;
    std::mutex m_lockTaskList;
    std::condition_variable m_condTaskList;
};

}