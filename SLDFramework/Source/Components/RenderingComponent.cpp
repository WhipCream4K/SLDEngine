#include "RenderingComponent.h"


//SLD::RenderingComponent::RenderingComponent(size_t elemSize, uint32_t elemCnt)
//	: m_PackageData()
//	//, m_RenderTarget()
//	, m_pCurrentDataPtr()
//	, m_ElementCnt()
//{
//	m_ElementCnt = elemCnt;
//	m_PackageData.reserve(elemSize + (size_t(elemCnt) * sizeof(RenderIdentifier)));
//	m_pCurrentDataPtr = m_PackageData.data();
//}

//void SLD::RenderingComponent::ReserveSize(size_t elemSize, uint32_t elemCnt)
//{
//	m_PackageData.reserve(elemSize * size_t(elemCnt) * sizeof(RenderIdentifier));
//}

//SLD::RenderingComponent::RenderingComponent(std::add_pointer_t<uint8_t> const& head, uint32_t offset, uint32_t elemCnt,
//	size_t elemSize)
//	: m_pBufferHead(head)
//	, m_MaxSize(elemSize)
//	, m_OffSet(offset)
//	, m_ElementCnt(elemCnt)
//{
//}
//
//SLD::RenderingComponent::RenderingComponent(std::add_pointer_t<void> const& head, uint32_t offset, uint32_t elemCnt,
//	size_t elemSize)
//	: m_pBufferHead(reinterpret_cast<uint8_t* const&>(head))
//	, m_MaxSize(elemSize)
//	, m_OffSet(offset)
//	, m_ElementCnt(elemCnt)
//{
//}

SLD::RenderingComponent::RenderingComponent(const RefPtr<ObservePtr<TransformComponent>>& transform,
	const RefPtr<uint8_t>& pointerToBuffer, size_t elemSize, uint32_t elemCnt)
	: m_Transform(transform)
	, m_PointToBuffer(pointerToBuffer)
	, m_MaxSize(elemSize)
	, m_UsedData()
	, m_ElementCnt(elemCnt)
{
}

WeakPtr<SLD::ObservePtr<SLD::TransformComponent>> SLD::RenderingComponent::GetTransform() const
{
	return m_Transform;
}

size_t SLD::RenderingComponent::GetMaxDataSize() const
{
	return m_MaxSize;
}

uint32_t SLD::RenderingComponent::GetElementCnt() const
{
	return m_ElementCnt;
}
