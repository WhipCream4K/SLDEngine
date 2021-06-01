
#ifndef SLDFRAMEWORK_RENDERING_COMPONENT_H
#define SLDFRAMEWORK_RENDERING_COMPONENT_H

#include "../Core/Base.h"
//#include "../Rendering/RenderTarget.h"

//#include "BaseComponent.h"
#include "NonTickComponent.h"
#include "../Rendering/RenderParams.h"

namespace SLD
{
	class TransformComponent;
	class RenderingComponent final : public NonTickComponent
	{
	public:

		static constexpr const char* UniqueId{ "RenderingComponent" };
		
		//RenderingComponent(
		//	const RefPtr<ObservePtr<TransformComponent>>& transform,
		//	std::add_pointer_t<uint8_t> const& head,
		//	size_t offset,
		//	size_t elemSize,
		//	uint32_t elemCnt);

		RenderingComponent(
			const RefPtr<ObservePtr<TransformComponent>>& transform,
			const RefPtr<uint8_t>& pointerToBuffer,
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
		uint32_t GetElementCnt() const;


	private:

		//std::vector<uint8_t> m_PackageData{};
		//ObservePtr<>
		//ObservePtr<>
		//RenderTarget m_RenderTarget;

		WeakPtr<ObservePtr<TransformComponent>> m_Transform;
		RefPtr<uint8_t> m_PointToBuffer;
		size_t m_MaxSize;
		size_t m_UsedData;
		uint32_t m_ElementCnt;
	};

	template <typename DataType>
	RefPtr<DataType> RenderingComponent::AllocData(RenderIdentifier identifier)
	{
		RefPtr<DataType> out{};
		if (m_UsedData + sizeof(DataType) <= m_MaxSize)
		{
			constexpr size_t renderIdSize{ sizeof(RenderIdentifier) };
			uint8_t* bufferStart{ m_PointToBuffer.get() + m_UsedData };
			size_t offsetFromStart{ m_UsedData };

			std::memcpy(bufferStart, &identifier, renderIdSize);
			m_UsedData += renderIdSize;

			out = RefPtr<DataType>{ reinterpret_cast<DataType*>(bufferStart + m_UsedData),[this,offsetFromStart](DataType* ptr)
			{
				uint8_t* delBuffer{m_PointToBuffer.get() + offsetFromStart};
				std::memset(delBuffer, 0, sizeof(DataType) + renderIdSize);
				ptr = nullptr;
			} };

			m_UsedData += sizeof(DataType);
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
		if (m_UsedData + sizeof(DataType) > m_MaxSize)
		{
			constexpr size_t renderIdSize{ sizeof(RenderIdentifier) };

			uint8_t* bufferSize{ m_PointToBuffer.get() + m_UsedData };
			size_t offsetFromStart{ m_UsedData };

			std::memcpy(bufferSize, &identifier, renderIdSize);
			m_UsedData += renderIdSize;

			DataType* allocPtr{ new (bufferSize + m_UsedData) DataType{std::forward<Args>(args)...} };
			out = RefPtr<DataType>{ allocPtr,[this,offsetFromStart](DataType* ptr)
			{
				uint8_t* delBuffer{m_PointToBuffer.get() + offsetFromStart};
				std::memset(delBuffer, 0, sizeof(DataType) + renderIdSize);
				ptr = nullptr;
			} };

			m_UsedData += sizeof(DataType);

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
		if(m_UsedData + sizeof(DataType) > m_MaxSize)
		{
			constexpr size_t renderIdSize{sizeof(RenderIdentifier)};
			uint8_t* bufferStart{ m_PointToBuffer.get() +m_UsedData };
			std::memcpy(bufferStart, &identifier, renderIdSize);
			m_UsedData += renderIdSize;
			std::memcpy(bufferStart + m_UsedData, &data, sizeof(DataType));
			m_UsedData += sizeof(DataType);
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