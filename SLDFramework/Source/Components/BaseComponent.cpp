#include "BaseComponent.h"
#include "../Core/WorldEntity.h"

void SLD::BaseComponent::MarkDestroy()
{
	m_IsMarkedDestroy = true;
}
