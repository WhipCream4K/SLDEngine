
#ifndef SLDFRAMEWORK_RENDERING_COMPONENT_H
#define SLDFRAMEWORK_RENDERING_COMPONENT_H

#include "../Core/Base.h"
#include "../Rendering/RenderTarget.h"

#include "../Rendering/RenderParams.h"

namespace SLD
{
	class TransformComponent;
	class RenderingComponent final
	{
	public:

		RenderingComponent() = default;

		//RenderingComponent(const rtm::matrix4x4f& worldMatrix,size_t elemSize,size_t elemCnt);
		RenderingComponent(size_t elemSize, uint32_t elemCnt);

		void ReserveSize(size_t elemSize, uint32_t elemCnt);

		template<typename DataType>
		RefPtr<DataType> AllocData(RenderIdentifier identifier);
		
		// can throw exceptions from access violation BE CAREFUL
		template<typename DataType>
		void PushElement(RenderIdentifier identifier, DataType data);

		size_t GetMaxDataSize() const { return m_PackageData.size(); }
		const uint8_t* GetRenderData() const noexcept { return m_PackageData.data(); }
		

	private:

		std::vector<uint8_t> m_PackageData{};
		RenderTarget m_RenderTarget;
		uint8_t* m_pCurrentDataPtr;
		uint32_t m_ElementCnt{};
	};

	template <typename DataType>
	RefPtr<DataType> RenderingComponent::AllocData(RenderIdentifier identifier)
	{
		std::memcpy(m_pCurrentDataPtr, &identifier, sizeof(RenderIdentifier));
		m_pCurrentDataPtr += sizeof(RenderIdentifier);
		
		RefPtr<DataType> out{ static_cast<DataType*>(m_pCurrentDataPtr),[](DataType* ptr)
		{
			ptr = nullptr;
		} };

		m_pCurrentDataPtr += sizeof(DataType);

		return out;
	}

	template <typename DataType>
	void RenderingComponent::PushElement(RenderIdentifier identifier, DataType data)
	{
		std::memcpy(m_pCurrentDataPtr, &identifier, sizeof(RenderIdentifier));
		m_pCurrentDataPtr += sizeof(RenderIdentifier);
		std::memcpy(m_pCurrentDataPtr, &data, sizeof(DataType));
		m_pCurrentDataPtr += sizeof(DataType);
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