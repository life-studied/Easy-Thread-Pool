#pragma once
#include <functional>
#include <thread>
#include <vector>
#include <memory>
#include <condition_variable>
#include <future>
#include "SafeQueue.h"
namespace pooltech
{
	class ThreadPool
	{
	public:
		ThreadPool(const ThreadPool&) = delete;
		ThreadPool(ThreadPool&&) = delete;
		ThreadPool& operator=(const ThreadPool&) = delete;
		ThreadPool& operator=(ThreadPool&&) = delete;
	public:
		ThreadPool();
		ThreadPool(int threadNum);
		~ThreadPool();
	public:
		template<typename Func, typename ...Args>
		auto submitTask(Func &&func,Args&&...args)
		{
			//1.���������ʹ������ת��������ȫ�����
			using returnType = decltype(func(std::forward<Args>(args)...));		//in C++17: using returnType = typename std::invoke_result<Func,Args...>::type;
			std::function<returnType()> taskWrapper1 = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);

			//2.������ֵ��ʹ��packaged_taskȥ������ֵ���ĳ�future����ȡ
			auto taskWrapper2 = std::make_shared<std::packaged_task<returnType()>>(taskWrapper1);
			TaskType wrapperFunction = [taskWrapper2]() {
				(*taskWrapper2)();
			};

			//������ӣ������߳�
			m_taskQueue.push(wrapperFunction);
			m_cv.notify_one();

			return taskWrapper2->get_future();
		}
	private:
		void initialize();
	private:
		using TaskType = std::function<void()>;

		SafeQueue<TaskType> m_taskQueue;
		std::vector<std::thread> m_threads;
		
		std::condition_variable m_cv;
		std::mutex m_mtx;

		std::atomic<bool> m_runningStatus;
	};
}
