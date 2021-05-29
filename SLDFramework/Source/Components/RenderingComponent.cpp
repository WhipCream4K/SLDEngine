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
	std::add_pointer_t<uint8_t> const& head, size_t offset, size_t elemSize, uint32_t elemCnt)
	: m_Transform(transform)
	, m_pBufferHead(head)
	, m_MaxSize(elemSize)
	, m_FromBufferHeadOffset(offset)
	, m_CurrentDataOffset()
	, m_ElementCnt(elemCnt)
{
}

SLD::RenderingComponent::RenderingComponent(const RefPtr<ObservePtr<TransformComponent>>& transform,
	std::add_pointer_t<void> const& head, size_t offset, size_t elemSize, uint32_t elemCnt)
	: m_Transform(transform)
	, m_pBufferHead((uint8_t* const&)head)
	, m_MaxSize(elemSize)
	, m_FromBufferHeadOffset(offset)
	, m_CurrentDataOffset()
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

uint8_t* SLD::RenderingComponent::GetRenderData() noexcept
{
	return m_pBufferHead + m_FromBufferHeadOffset;
}

uint32_t SLD::RenderingComponent::GetElementCnt() const
{
	return m_ElementCnt;
}

const uint8_t* SLD::RenderingComponent::GetRenderData() const noexcept
{
	return m_pBufferHead + m_FromBufferHeadOffset;
}
