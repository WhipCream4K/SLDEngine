#include "RenderingComponent.h"


SLD::RenderingComponent::RenderingComponent(size_t elemSize, uint32_t elemCnt)
	: m_PackageData()
	, m_RenderTarget()
	, m_pCurrentDataPtr()
	, m_ElementCnt()
{
	m_ElementCnt = elemCnt;
	m_PackageData.reserve(elemSize + (size_t(elemCnt) * sizeof(RenderIdentifier)));
	m_pCurrentDataPtr = m_PackageData.data();
}

void SLD::RenderingComponent::ReserveSize(size_t elemSize, uint32_t elemCnt)
{
	m_PackageData.reserve(elemSize * size_t(elemCnt) * sizeof(RenderIdentifier));
}

uint32_t SLD::RenderingComponent::GetElementCnt() const
{
	return m_ElementCnt;
}
