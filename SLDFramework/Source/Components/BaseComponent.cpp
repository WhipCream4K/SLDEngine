#include "BaseComponent.h"
#include "../Core/WorldEntity.h"

void SLD::BaseComponent::MarkDestroy()
{
	m_IsMarkedDestroy = true;
}

WeakPtr<SLD::GameObject> SLD::BaseComponent::GetParent() const noexcept
{
	return m_Parent;
}

void SLD::BaseComponent::SetParent(const RefPtr<GameObject>& parent)
{
	m_Parent = parent;
}
