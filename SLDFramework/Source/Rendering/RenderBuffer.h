
#ifndef SLDFRAMEWORK_RENDERQUEUE_H
#define SLDFRAMEWORK_RENDERQUEUE_H

#include "../Core/Base.h"
#include "RenderParams.h"
//#include "../Core/PMR_Resource/UnSynchronizedSpatialResource.h"

namespace SLD
{
	using Tracker = uint8_t*;

	class TransformComponent;
	class RenderBuffer
	{

	public:

		RenderBuffer();
		RenderBuffer(size_t startSize);

		void PushRenderElement(uint8_t* buffer, size_t size);

		template<typename DataType>
			[[nodiscard]] RefPtr<Tracker> PushRenderElement(const RefPtr<ObservePtr<TransformComponent>>& transform, RenderIdentifier id, DataType* data);

		template<typename DataType>
			[[nodiscard]] RefPtr<Tracker> AllocRenderElement(const RefPtr<ObservePtr<TransformComponent>>& transform, RenderIdentifier id);

		//void PushRenderElement(ObservePtr<TransformComponent>* transform,)
		void Resize(size_t count);
		void Clear();

		uint8_t* GetBufferStart() const noexcept;
		size_t GetBufferCapacity() const noexcept;
		size_t GetBufferSize() const noexcept;
		uint32_t GetElementCnt() const noexcept;

	private:

		void EraseWatcherThenShift(uint8_t* where,size_t dataSize);
		void ShifBuffer(size_t newSize);

		//std::vector<uint8_t> m_RenderStack;
		OwnedPtr<uint8_t[]>		m_MainBuffer;
		size_t					m_CurrentBufferSize;
		size_t					m_BufferCapacity;
		uint32_t				m_RenderElemCnt{};
		std::list<uint8_t*>		m_REWatcher;
		//uint8_t* m_pCurrent{};
	};

	template <typename DataType>
	RefPtr<Tracker> RenderBuffer::PushRenderElement(const RefPtr<ObservePtr<TransformComponent>>& transform,
		RenderIdentifier id, DataType* data)
	{
		m_RenderElemCnt++;
		
		constexpr size_t totalDataBuffer{ sizeof(void*) + sizeof(RenderIdentifier) + sizeof(DataType) };
		
		if (m_CurrentBufferSize + totalDataBuffer > m_BufferCapacity)
			Resize(m_CurrentBufferSize + totalDataBuffer);
		
		uint8_t* buffer{ m_MainBuffer.get() };

		// Transform
		ObservePtr<TransformComponent>* temp{ transform.get() };
		//std::memcpy(buffer + m_CurrentBufferSize, &temp, sizeof(void*));
		std::copy_n((uint8_t*)&temp, sizeof(void*), buffer + m_CurrentBufferSize);
		m_CurrentBufferSize += sizeof(void*);

		// Id
		//std::memcpy(buffer + m_CurrentBufferSize, &id, sizeof(RenderIdentifier));
		std::copy_n((uint8_t*)&id, sizeof(RenderIdentifier), buffer + m_CurrentBufferSize);
		m_CurrentBufferSize += sizeof(RenderIdentifier);

		// Data
		auto& ptrAddress{ m_REWatcher.emplace_back(buffer + m_CurrentBufferSize) };
		std::copy_n((uint8_t*)data, sizeof(DataType), buffer + m_CurrentBufferSize);
		m_CurrentBufferSize += sizeof(DataType);

		auto out{ RefPtr<Tracker>{&ptrAddress,[this](Tracker* ptr)
		{
			EraseWatcherThenShift(*ptr,sizeof(DataType));
		}} };

		return out;
	}

	template <typename DataType>
	RefPtr<Tracker> RenderBuffer::AllocRenderElement(const RefPtr<ObservePtr<TransformComponent>>& transform,
		RenderIdentifier id)
	{
		m_RenderElemCnt++;

		constexpr size_t totalDataBuffer{ sizeof(void*) + sizeof(RenderIdentifier) + sizeof(DataType) };

		if (m_CurrentBufferSize + totalDataBuffer > m_BufferCapacity)
			Resize(m_CurrentBufferSize + totalDataBuffer);

		uint8_t* buffer{ m_MainBuffer.get() };

		// Transform
		ObservePtr<TransformComponent>* temp{ transform.get() };
		//std::memcpy(buffer + m_CurrentBufferSize, &temp, sizeof(void*));
		std::copy_n((uint8_t*)&temp, sizeof(void*), buffer + m_CurrentBufferSize);
		m_CurrentBufferSize += sizeof(void*);

		// Id
		//std::memcpy(buffer + m_CurrentBufferSize, &id, sizeof(RenderIdentifier));
		std::copy_n((uint8_t*)&id, sizeof(RenderIdentifier), buffer + m_CurrentBufferSize);
		m_CurrentBufferSize += sizeof(RenderIdentifier);

		// Data
		auto& ptrAddress{ m_REWatcher.emplace_back(buffer + m_CurrentBufferSize) };
		//std::copy_n((uint8_t*)data, sizeof(DataType), buffer + m_CurrentBufferSize);
		m_CurrentBufferSize += sizeof(DataType);

		auto out{ RefPtr<Tracker>{&ptrAddress,[this](Tracker* ptr)
		{
			EraseWatcherThenShift(*ptr,sizeof(DataType));
		}} };

		return out;
	}
}

template<typename DataType>
inline void CopyTextureRegion(DataType* from, const RefPtr<SLD::Tracker>& tracker)
{
	std::copy_n((uint8_t*)from, sizeof(DataType), *tracker.get());
}

#endif
