
#ifndef SLDFRAMEWORK_RENDERING_COMPONENT_H
#define SLDFRAMEWORK_RENDERING_COMPONENT_H

#include "../Core/Base.h"
//#include "../Rendering/RenderTarget.h"

#include "../Rendering/RenderParams.h"

namespace SLD
{
	class TransformComponent;
	class RenderingComponent final
	{
	public:

		// TODO: Make Render buffer updates dynamically
		//RenderingComponent() = default;

		RenderingComponent(
			const RefPtr<ObservePtr<TransformComponent>>& transform,
			std::add_pointer_t<uint8_t> const& head,
			size_t offset,
			size_t elemSize,
			uint32_t elemCnt);

		RenderingComponent(
			const RefPtr<ObservePtr<TransformComponent>>& transform,
			std::add_pointer_t<void> const& head,
			size_t offset,
			size_t elemSize,
			uint32_t elemCnt
		);

		template<typename DataType>
		RefPtr<DataType> AllocData(RenderIdentifier identifier);

		template<typename DataType, typename ...Args>
		RefPtr<DataType> AllocAndConstructData(RenderIdentifier identifier, Args&&... args);

		// can throw exceptions from access violation BE CAREFUL
		template<typename DataType>
		void PushElement(RenderIdentifier identifier, DataType data);

		[[nodiscard]] WeakPtr<ObservePtr<TransformComponent>> GetTransform() const;

		size_t GetMaxDataSize() const;
		uint8_t* GetRenderData() noexcept;
		uint32_t GetElementCnt() const;
		const uint8_t* GetRenderData() const noexcept;


	private:

		//std::vector<uint8_t> m_PackageData{};
		//ObservePtr<>
		//ObservePtr<>
		//RenderTarget m_RenderTarget;

		WeakPtr<ObservePtr<TransformComponent>> m_Transform;
		uint8_t* const& m_pBufferHead;
		size_t m_MaxSize;
		size_t m_FromBufferHeadOffset;
		size_t m_CurrentDataOffset;
		uint32_t m_ElementCnt;
	};

	template <typename DataType>
	RefPtr<DataType> RenderingComponent::AllocData(RenderIdentifier identifier)
	{
		RefPtr<DataType> out{};
		if (m_CurrentDataOffset + sizeof(DataType) > m_MaxSize)
		{
			constexpr size_t renderIdSize{ sizeof(RenderIdentifier) };

			uint8_t* bufferStart{ m_pBufferHead + m_FromBufferHeadOffset + m_CurrentDataOffset };
			size_t offsetFromStart{ m_CurrentDataOffset };

			std::memcpy(bufferStart, &identifier, renderIdSize);
			m_CurrentDataOffset += renderIdSize;

			out = RefPtr<DataType>{ reinterpret_cast<DataType*>(bufferStart + m_CurrentDataOffset),[this,offsetFromStart](DataType* ptr)
			{
				uint8_t* delBuffer{m_pBufferHead + m_FromBufferHeadOffset + offsetFromStart};
				std::memset(delBuffer, 0, sizeof(DataType) + renderIdSize);
				ptr = nullptr;
			} };

			m_CurrentDataOffset += sizeof(DataType);
		}

		//std::memcpy(m_pCurrentDataPtr, &identifier, sizeof(RenderIdentifier));
		//m_pCurrentDataPtr += sizeof(RenderIdentifier);
		//
		//RefPtr<DataType> out{ reinterpret_cast<DataType*>(m_pCurrentDataPtr),[](DataType* ptr)
		//{
		//	ptr = nullptr;
		//} };

		//m_pCurrentDataPtr += sizeof(DataType);
		//
		return out;
	}

	template <typename DataType, typename ... Args>
	RefPtr<DataType> RenderingComponent::AllocAndConstructData(RenderIdentifier identifier, Args&&... args)
	{
		RefPtr<DataType> out{};
		if (m_CurrentDataOffset + sizeof(DataType) > m_MaxSize)
		{
			constexpr size_t renderIdSize{ sizeof(RenderIdentifier) };

			uint8_t* bufferSize{ m_pBufferHead + m_FromBufferHeadOffset + m_CurrentDataOffset };
			size_t offsetFromStart{ m_CurrentDataOffset };

			std::memcpy(bufferSize, &identifier, renderIdSize);
			m_CurrentDataOffset += renderIdSize;

			DataType* allocPtr{ new (bufferSize + m_CurrentDataOffset) DataType{std::forward<Args>(args)...} };
			out = RefPtr<DataType>{ allocPtr,[this,offsetFromStart](DataType* ptr)
			{
				uint8_t* delBuffer{m_pBufferHead + m_FromBufferHeadOffset + offsetFromStart};
				std::memset(delBuffer, 0, sizeof(DataType) + renderIdSize);
				ptr = nullptr;
			} };

			m_CurrentDataOffset += sizeof(DataType);

		}

		//std::memcpy(m_pCurrentDataPtr, &identifier, sizeof(RenderIdentifier));
		//m_pCurrentDataPtr += sizeof(RenderIdentifier);

		//DataType* out{ new (m_pCurrentDataPtr) DataType{std::forward<Args>(args)...} };
		//RefPtr<DataType> smrtOut{ out,[](DataType* ptr)
		//{
		//	ptr = nullptr;
		//} };

		//m_pCurrentDataPtr += sizeof(DataType);

		//return smrtOut;

		return out;
	}

	template <typename DataType>
	void RenderingComponent::PushElement(RenderIdentifier identifier, DataType data)
	{
		if(m_CurrentDataOffset + sizeof(DataType) > m_MaxSize)
		{
			constexpr size_t renderIdSize{sizeof(RenderIdentifier)};
			uint8_t* bufferStart{ m_pBufferHead + m_FromBufferHeadOffset + m_CurrentDataOffset };
			std::memcpy(bufferStart, &identifier, renderIdSize);
			m_CurrentDataOffset += renderIdSize;
			std::memcpy(bufferStart + m_CurrentDataOffset, &data, sizeof(DataType));
			m_CurrentDataOffset += sizeof(DataType);
		}
		//std::memcpy(m_pCurrentDataPtr, &identifier, sizeof(RenderIdentifier));
		//m_pCurrentDataPtr += sizeof(RenderIdentifier);
		//std::memcpy(m_pCurrentDataPtr, &data, sizeof(DataType));
		//m_pCurrentDataPtr += sizeof(DataType);
	}

	//template <size_t PackageSize, size_t ElemCnt>
	//template <typename DataType>
	//void RenderingComponent<PackageSize, ElemCnt>::PushElement(RenderIdentifier identifier, DataType data)
	//{
	//	std::memcpy(m_pCurrentDataPtr, &identifier, sizeof(RenderIdentifier));
	//	m_pCurrentDataPtr += sizeof(RenderIdentifier);
	//	std::memcpy(m_pCurrentDataPtr, &data, sizeof(DataType));
	//	m_pCurrentDataPtr += sizeof(DataType);
	//}
}

#endif