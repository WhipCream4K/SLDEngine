
#ifndef SLDFRAMEWORK_ASYNCSYSTEMT_H
#define SLDFRAMEWORK_ASYNCSYSTEMT_H

#include "SystemTemplate.h"
#include "../Tracer/minitrace.h"

namespace SLD
{
	template<typename ...Args>
	class AsyncSystemT :
		public SystemTemplate<Args...>
	{
	public:

		AsyncSystemT(WorldEntity& world,PipelineLayer layer = PipelineLayer::OnAsync);
		
		virtual void OnUpdate(GameObjectId gameObjectId, float deltaTime, Args*...) = 0;

	protected:
		
		virtual void CallUpdate(
			const std::vector<GameObjectId>& gameObjectsIds,
			std::array<size_t, sizeof...(Args)>& strideOffsets,
			float deltaTime,
			uint8_t* componentAddress,
			int componentStructSize) override;
		
		virtual void FetchFutureResults() override;

	private:

		std::vector<std::future<void>> m_FutureResults;
	};

	template <typename ... Args>
	AsyncSystemT<Args...>::AsyncSystemT(WorldEntity& world, PipelineLayer layer)
		: SystemTemplate<Args...>(world, layer)
	{
	}

	template <typename ... Args>
	void AsyncSystemT<Args...>::CallUpdate(const std::vector<GameObjectId>& gameObjectsIds,
		std::array<size_t, sizeof...(Args)>& strideOffsets, float deltaTime, uint8_t* componentAddress,
		int componentStructSize)
	{
		WorldEntity& world{ this->m_World };

		if(m_FutureResults.size() < gameObjectsIds.size())
			m_FutureResults.resize(gameObjectsIds.size());
		

		size_t activeFuture{};
		for (const auto& id : gameObjectsIds)
		{
			m_FutureResults[activeFuture++] = std::move(world.PushAsyncTask([this, &deltaTime, strideOffsets, componentAddress, &id]()
				{
					this->ApplyPack(id, deltaTime, strideOffsets, componentAddress, std::index_sequence_for<Args...>());
				}));
			
			componentAddress += componentStructSize;
		}
	
	}

	template <typename ... Args>
	void AsyncSystemT<Args...>::FetchFutureResults()
	{
		MTR_SCOPE("main", "fetch future");
		
		for (auto& future : m_FutureResults)
		{
			if (future.valid())
				future.get();
		}

		//if(!m_FutureResults.empty())
		//	m_FutureResults.clear();
	}
}


#endif
