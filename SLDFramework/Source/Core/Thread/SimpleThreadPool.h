
#ifndef SLDFRAMEWORK_THREADPOOL_H
#define SLDFRAMEWORK_THREADPOOL_H

#include "../Base.h"
#include "../ScriptableChecks.h"
#include <thread>

namespace SLD
{
	class SimpleThreadPool
	{
	public:

		SimpleThreadPool(size_t threadCount = (size_t)std::thread::hardware_concurrency());
		~SimpleThreadPool();

		template<typename Func, typename ...Args,
			typename Ret = std::invoke_result_t<Func, Args...>>
			std::future<Ret> PushTask(Func&& fn, Args&&... args);

	private:

		void Run();


		std::vector<std::future<void>> m_Workers;
		std::queue<std::packaged_task<void()>> m_Tasks;
		std::condition_variable m_TaskWait;
		std::mutex m_Mutex;
	};

	template <typename Func, typename ... Args, typename Ret>
	std::future<Ret> SimpleThreadPool::PushTask(Func&& fn, Args&&... args)
	{
		std::packaged_task<Ret()> task{};

		if constexpr (sizeof...(Args) > 0)
		{
			task = std::move(std::packaged_task<Ret()>{ [&fn, &args...] ()
			{
				fn(std::forward<Args>(args)...);
			}});
		}
		else
		{
			task = std::move(std::packaged_task<Ret()>(std::forward<Func>(fn)));
		}


		std::future<Ret> future{ task.get_future() };

		{
			std::unique_lock lock{ m_Mutex };
			m_Tasks.emplace(std::move(task));
		}

		m_TaskWait.notify_one();

		return future;
	}

}


#endif
