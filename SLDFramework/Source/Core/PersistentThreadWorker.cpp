#include "PersistentThreadWorker.h"

void SLD::PersistentThreadWorker::Start()
{
	m_Task = std::async(std::launch::async, &PersistentThreadWorker::ThreadTask, this);
}

void SLD::PersistentThreadWorker::Wake()
{
	if (!m_BarrierFuture.valid())
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

SLD::PersistentThreadWorker::PersistentThreadWorker(PersistentThreadWorker&& other) noexcept
	: m_Task(std::move(other.m_Task))
	, m_Barrier(std::move(other.m_Barrier))
	, m_BarrierFuture(std::move(other.m_BarrierFuture))
	, m_Trigger(std::move(other.m_Trigger))
	, m_IsExit(std::move(other.m_IsExit))
{
}

SLD::PersistentThreadWorker& SLD::PersistentThreadWorker::operator=(PersistentThreadWorker&& other) noexcept
{
	if(this != &other)
	{
		m_Task = std::move(other.m_Task);
		m_Barrier = std::move(other.m_Barrier);
		m_BarrierFuture = std::move(other.m_BarrierFuture);
		m_Trigger = other.m_Trigger;
		m_IsExit = other.m_IsExit;
	}

	return *this;
}

SLD::PersistentThreadWorker::~PersistentThreadWorker()
{
	m_IsExit = true;
}

void SLD::PersistentThreadWorker::ThreadTask()
{
	while (!m_IsExit)
	{
		if (m_Trigger)
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
