#include "ThreadPool.h"
using namespace pooltech;

pooltech::ThreadPool::ThreadPool() : ThreadPool(std::thread::hardware_concurrency())
{
}

pooltech::ThreadPool::ThreadPool(int threadNum) : m_threads(threadNum), m_runningStatus(true)
{
	initialize();
}

pooltech::ThreadPool::~ThreadPool()
{
	m_runningStatus = false;
	m_cv.notify_all();
	for (auto& thread : m_threads)
	{
		if(thread.joinable())
			thread.join();
	}

}

void pooltech::ThreadPool::initialize()
{
	for (int i = 0;i<m_threads.size();i++)
	{
		auto worker = [this, i]() {
			while (m_runningStatus)
			{
				TaskType task;
				bool isSuccess = false;

				{
					std::unique_lock<std::mutex> lock(m_mtx);
					if (this->m_taskQueue.empty())
					{
						this->m_cv.wait(lock);
					}
					isSuccess = this->m_taskQueue.pop(task);
				}

				if (isSuccess)
				{
					task();
				}
			}
		};
		m_threads[i] = std::thread(worker);
	}
}
