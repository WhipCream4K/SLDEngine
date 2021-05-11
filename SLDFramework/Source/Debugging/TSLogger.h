
#ifndef SLDFRAMEWORK_TSLOGGER_H
#define SLDFRAMEWORK_TSLOGGER_H

#include <future>
#include <queue>

namespace SLD
{
	class TSLogger
	{
	public:

		using TaskFnType = void();
		
		static TSLogger& GetInstance()
		{
			static TSLogger instance{};
			return instance;
		}

		//static std::shared_ptr<TSLogger> GetInstance()
		//{
		//	if (m_Instance)
		//		return m_Instance;
		//	m_Instance = std::make_shared<TSLogger>();
		//	return m_Instance;
		//}

		static void Init()
		{
			// start a work on new thread, run in parallel
			GetInstance().Start();
		}

		static size_t GetFrameworkHeapSize()
		{
			return GetInstance().m_FrameworkReservedHeapSize;
		}
		
		// NOTE: Don't ever use this in application run-time
		// This is not thread-safe
		static void FrameworkAlloc(size_t size)
		{
			GetInstance().m_FrameworkReservedHeapSize += size;
		}
		
		// NOTE: Don't ever use this in application run-time
		// This is not thread-safe
		static void FrameworkDealloc(size_t size)
		{
			GetInstance().m_FrameworkReservedHeapSize -= size;
		}

		static void IncreaseOccupiedHeap(size_t size)
		{
			GetInstance().m_OccupiedHeap.fetch_add(size);
		}

		static void DecreaseOccupiedHeap(size_t size)
		{
			GetInstance().m_OccupiedHeap.fetch_add(size);
		}
		
		~TSLogger() = default;
		
		TSLogger(const TSLogger&) = delete;
		TSLogger& operator=(const TSLogger&) = delete;
		TSLogger(TSLogger&&) = delete;
		TSLogger& operator=(TSLogger&&) = delete;

	private:

		[[noreturn]] void ThreadTask();

		void QueueWorkLoad(const std::function<TaskFnType>& fnPtr);

		void Start();

	private:

		TSLogger() {}
				
		std::future<void> m_FutureObj;
		std::condition_variable m_Trigger;
		std::mutex m_Mutex;
		std::queue<std::function<TaskFnType>> m_WorkLoad;

		// NOTE: Important keep track only initialization time 
		std::size_t m_FrameworkReservedHeapSize{};
		std::atomic_size_t m_OccupiedHeap{};
	};
}

#define SLD_LOG_TRACE_HEAP_ALLOC(SIZE) SLD::TSLogger::FrameworkAlloc(SIZE)
#define SLD_LOG_UNTRACE_HEAP_ALLOC(SIZE) SLD::TSLogger::FrameworkDealloc(SIZE)
#define SLD_LOG_TRACE_USED_HEAP(SIZE) SLD::TSLogger::IncreaseOccupiedHeap(SIZE)
#define SLD_LOG_UNTRACE_USED_HEAP(SIZE) SLD::TSLogger::DecreaseOccupiedHeap(SIZE)
#define SLD_LOG_INIT() SLD::TSLogger::Init()

#endif