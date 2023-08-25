#pragma once
#include <queue>
#include <mutex>
template<typename T>
class SafeQueue
{
public:
	SafeQueue() = default;
	~SafeQueue() = default;

public:
	SafeQueue(const SafeQueue&) = delete;
	SafeQueue(SafeQueue&&) = delete;
	SafeQueue(const SafeQueue&&) = delete;
	SafeQueue& operator=(const SafeQueue&) = delete;
	SafeQueue& operator=(const SafeQueue&&) = delete;

private:
	std::queue<T> m_queue;
	std::mutex m_mtx;

public:
	int size()
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		return m_queue.size();
	}

	bool empty()
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		return m_queue.empty();
	}

	void push(const T& value)
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		m_queue.push(value);
	}

	void push(T&& value)
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		m_queue.push(std::move(value));
	}

	bool pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m_mtx);
		if (m_queue.empty())
			return false;
		
		value = std::move(m_queue.front());
		m_queue.pop();
		return true;
	}
};

