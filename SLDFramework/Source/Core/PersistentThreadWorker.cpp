#include "PersistentThreadWorker.h"

void SLD::PersistentThreadWorker::Start()
{
	m_Task = std::async(std::launch::async, &PersistentThreadWorker::ThreadTask, this);
}

void SLD::PersistentThreadWorker::Wake()
{
	if(!m_BarrierFuture.valid())
	{
		// create new promise 
		m_Barrier = std::promise<void>{};
		m_BarrierFuture = m_Barrier.get_future();
		m_Trigger = true;
	}
}

void SLD::PersistentThreadWorker::WaitTillFinished()
{
	m_BarrierFuture.get();
}

void SLD::PersistentThreadWorker::ThreadTask()
{
	while(true)
	{
		if(m_Trigger)
		{
			for (auto& task : m_WorkLoads)
			{
				task();
			}

			m_Trigger = false;
			m_Barrier.set_value();
		}
	}
}
