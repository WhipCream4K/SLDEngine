#include "RenderingComponent.h"

#include "TransformComponent.h"


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

//SLD::RenderingComponent::RenderingComponent(const SharedPtr<ObservePtr<TransformComponent>>& transform,
//	const SharedPtr<uint8_t*>& pointerToBuffer, const ObservePtr<std::nullptr_t>& bufferObserver, size_t elemSize,
//	uint32_t elemCnt)
//	: m_Transform(transform)
//	, m_PointToBuffer(pointerToBuffer)
//	, m_BufferObserver(bufferObserver)
//	, m_MaxSize(elemSize)
//	, m_UsedData()
//	, m_ElementCnt(elemCnt)
//{
//	ObservePtr<TransformComponent>* correct{ transform.get() };
//	std::memcpy(m_PointToBuffer.get(), &correct, sizeof(void*));
//	m_UsedData += sizeof(void*);
//}

SLD::RenderingComponent::RenderingComponent(const SharedPtr<ObservePtr<TransformComponent>>& transform,
	const SharedPtr<uint8_t>& pointerToBuffer, const ObservePtr<std::nullptr_t>& bufferObserver, size_t elemSize,
	uint32_t elemCnt)
	: m_Transform(transform)
	, m_PointToBuffer(pointerToBuffer)
	, m_BufferObserver(bufferObserver)
	, m_MaxSize(elemSize)
	, m_UsedData()
	, m_ElementCnt(elemCnt)
{
	ObservePtr<TransformComponent>* correct{ transform.get() };
	uint8_t* temp{};
	std::memcpy(&temp, m_PointToBuffer.get(), sizeof(void*));
	std::memcpy(temp, &correct, sizeof(void*));
	m_UsedData += sizeof(void*);
}

SLD::RenderingComponent::RenderingComponent(size_t elemSize, uint32_t elemCnt)
	: m_ElementCnt(elemCnt)
{
	m_PackageData.reserve(elemSize);
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
