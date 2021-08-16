#pragma once

#include <SLDFramework.h>
#include <System/SystemTemplate.h>
#include "MyComponents.h"

class HandleSpriteOutOfWindow : public SLD::SystemTemplate<SLD::TransformComponent,SpriteRenderComponent>
{
public:

	HandleSpriteOutOfWindow(SLDWorldEntity& world, const SLD::Vector2<uint32_t>& windowDimension);
	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent*, SpriteRenderComponent*) override;


private:

	SLD::Vector2<uint32_t> m_Window;
	float m_Right{};
	float m_Left{};
	float m_Up{};
	float m_Down{};
};

