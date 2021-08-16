#include "HandleSpriteOutOfWindow.h"

#include "Components/TransformComponent.h"

HandleSpriteOutOfWindow::HandleSpriteOutOfWindow(SLDWorldEntity& world, const SLD::Vector2<uint32_t>& windowDimension)
	: SystemTemplate(world,SLD::PipelineLayer::OnPreRender)
	, m_Window(windowDimension)
{
	m_Left = -float(windowDimension.x) / 2.0f;
	m_Right = windowDimension.x / 2.0f;
	m_Up = windowDimension.y / 2.0f;
	m_Down = -float(windowDimension.y)/ 2.0f;
}

void HandleSpriteOutOfWindow::OnUpdate(SLD::GameObjectId , float , SLD::TransformComponent* transform,
	SpriteRenderComponent* sprite)
{
	const auto& worldPos{ transform->GetWorldPos() };

	bool shouldDisable{};
	
	// left
	if (worldPos.x < m_Left)
		shouldDisable = true;
	// right
	else if (worldPos.x > m_Right)
		shouldDisable = true;

	// Up
	if (worldPos.y > m_Up)
		shouldDisable = true;
	// Down
	else if (worldPos.y < m_Down)
		shouldDisable = true;

	sprite->shouldRender = shouldDisable ? false : true;
}
