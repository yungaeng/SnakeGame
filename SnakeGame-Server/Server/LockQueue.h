#pragma once

template<typename T>
class LockQueue {
private:
	std::queue<T>	m_queue;
	std::mutex		m_mutex;

public:
	void Push(T item)
	{
		std::lock_guard<std::mutex> lk{ m_mutex };
		m_queue.push(item);
	}

	T Pop()
	{
		std::lock_guard<std::mutex> lk{ m_mutex };
		if(m_queue.empty()) return T{};
		T item = m_queue.front();
		m_queue.pop();
		return item;
	}

	bool Empty()
	{
		std::lock_guard<std::mutex> lk{ m_mutex };
		return m_queue.empty();
	}
};

