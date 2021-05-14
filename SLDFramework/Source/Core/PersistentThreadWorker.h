
#ifndef SLDFRAMEWORK_THREADWORKER_H
#define SLDFRAMEWORK_THREADWORKER_H

#include <future>
#include <mutex>
#include <queue>
#include <functional>

namespace SLD
{
	class PersistentThreadWorker
	{
	public:

		PersistentThreadWorker() = default;
		
		void Start();

		template<typename FnType,typename Ret = std::result_of_t<FnType&()>>
		[[noreturn]] void AssignTask(FnType&& task);

		//template<typename FnType,typename Object,typename Ret = std::result_of_t<FnType>>
		//[[maybe_unused]] std::future<Ret> AssignTask(FnType&& task, Object& obj);

		void Wake();

		void WaitTillFinished();
	
	private:
		void ThreadTask();
	private:
		std::future<void> m_Task;
		std::vector<std::function<void()>> m_WorkLoads;
		std::promise<void> m_Barrier;
		std::future<void> m_BarrierFuture;
		bool m_Trigger{};
	};

	template <typename FnType, typename Ret>
	void PersistentThreadWorker::AssignTask(FnType&& task)
	{
		//std::packaged_task<Ret()> pTask{std::forward<FnType>(task)};
		//std::future<Ret> work{pTask.get_future()};
		m_WorkLoads.emplace_back(std::move(task));
	}
}

#endif