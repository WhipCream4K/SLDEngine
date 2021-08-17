#include "SimpleThreadPool.h"

SLD::SimpleThreadPool::SimpleThreadPool(size_t threadCount)
	: m_Workers()
	, m_Tasks()
	, m_Mutex()
{
	for (size_t i = 0; i < threadCount; ++i)
	{
		m_Workers.emplace_back(std::async(std::launch::async, &SimpleThreadPool::Run, this));
	}
}

SLD::SimpleThreadPool::~SimpleThreadPool()
{
	{
		std::unique_lock lock{ m_Mutex };
		for (size_t i = 0; i < m_Workers.size(); ++i)
		{
			m_Tasks.push({});
		}
	}

	m_TaskWait.notify_all();
	m_Workers.clear();
}

void SLD::SimpleThreadPool::Run()
{
	while (true)
	{
		std::packaged_task<void()> task{};

		{
			std::unique_lock lock{ m_Mutex };

			m_TaskWait.wait(lock, [this]()
				{
					return !m_Tasks.empty();
				});

			task = std::move(m_Tasks.front());
			m_Tasks.pop();
		}

		if (!task.valid()) return;

		const size_t x{ m_Tasks.size() };
		x;
		task();
	}
}
