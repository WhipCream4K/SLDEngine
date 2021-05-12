#include "TSLogger.h"
#include <chrono>

//std::shared_ptr<SLD::TSLogger> SLD::TSLogger::m_Instance{};

// TODO: Stop This Thread when app exits
void SLD::TSLogger::ThreadTask()
{
	while (true)
	{
		std::function<TaskFnType> task{};

		// check for work load
		{
			std::unique_lock<std::mutex> uLock{ m_Mutex };
			if (m_WorkLoad.empty())
				// wait for signal from workload and another predicate to double check for 
				// validity if the workload actually has work in it
				m_Trigger.wait(uLock, [this]() {return !m_WorkLoad.empty(); });

			task = std::move(m_WorkLoad.front());
			m_WorkLoad.pop();
		}

		// normally need to check for invalid here
		// but for this logger all the task will not get stop
		task();
	}
}

void SLD::TSLogger::QueueWorkLoad(const std::function<TaskFnType>& fnPtr)
{
	{
		std::scoped_lock<std::mutex> sLock{ m_Mutex };
		m_WorkLoad.push(fnPtr);
	}

	// wake the thread
	m_Trigger.notify_one();
}

void SLD::TSLogger::Start()
{
	//m_FutureObj = std::async(std::launch::async, &TSLogger::ThreadTask, this);
}
