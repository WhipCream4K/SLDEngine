
#ifndef SLDFRAMEWORK_THREADWORKER_H
#define SLDFRAMEWORK_THREADWORKER_H

#include <future>
#include <mutex>
#include <queue>
#include "ScriptableChecks.h"

namespace SLD
{
	class PersistentThreadWorker
	{
	public:

		
		void Start();

		template<typename FnType,
		typename Ret = std::invoke_result_t<FnType>>
		[[noreturn]] void AssignTask(FnType&& task);

		template<typename FnType,
			typename Ret = typename Function_Traits<FnType>::return_type,
			typename Class = typename Function_Traits<FnType>::element_type,
			typename ...Args>
			[[noreturn]] void AssignTask(FnType&& task, Class& caller, Args&&... args);


		// NOTE: Can only call once in the main thread
		// Not thread safe
		void Wake();

		void WaitTillFinished();

		PersistentThreadWorker() = default;
		PersistentThreadWorker(const PersistentThreadWorker& other) = delete;
		PersistentThreadWorker& operator=(const PersistentThreadWorker& other) = delete;
		PersistentThreadWorker(PersistentThreadWorker&& other) noexcept;
		PersistentThreadWorker& operator=(PersistentThreadWorker&& other) noexcept;
		~PersistentThreadWorker();
	
	private:
		void ThreadTask();
	private:
		std::future<void> m_Task;
		std::vector<std::function<void()>> m_WorkLoads;
		std::promise<void> m_Barrier;
		std::future<void> m_BarrierFuture;
		bool m_Trigger{};
		bool m_IsExit{};
	};

	template <typename FnType, typename Ret>
	void PersistentThreadWorker::AssignTask(FnType&& task)
	{
		m_WorkLoads.emplace_back(std::move(task));
	}

	template <typename FnType, typename Ret, typename Class, typename ... Args>
	void PersistentThreadWorker::AssignTask(FnType&& task, Class& caller, Args&&... args)
	{
		m_WorkLoads.emplace_back([task,&caller,&args...]()
		{
			//std::invoke(task, caller, std::forward<Args>(args)...);
			(caller.*task)(std::forward<Args>(args)...);
		});
	}
}

#endif