#include "LimitPlayerToPlayArea.h"

//LimitPlayerToPlayArea::LimitPlayerToPlayArea(SLD::WorldEntity& world)
//	: SystemTemplate(world,SLD::PipelineLayer::OnPreRender)
//	, m_AreaRect({0.0f,0.0f,100.0f,100.0f})
//{
//	
//}

LimitPlayerToPlayArea::LimitPlayerToPlayArea(SLD::WorldEntity& world, const rtm::float4f& playArea)
	: SystemTemplate(world, SLD::PipelineLayer::OnPreRender)
	, m_AreaRect(playArea)
{
	m_Left = m_AreaRect.x - m_AreaRect.z / 2.0f;
	m_Right = m_AreaRect.x + m_AreaRect.z / 2.0f;
	m_Up = m_AreaRect.y + m_AreaRect.w / 2.0f;
	m_Down = m_AreaRect.y - m_AreaRect.w / 2.0f;
}

void LimitPlayerToPlayArea::OnUpdate(SLD::GameObjectId, float, SLD::TransformComponent* transform,
	PlayerTag*)
{
	using namespace SLD;

	rtm::float3f currentPos{ transform->GetWorldPos() };

	bool shouldUpdate{};

	// to the left
	if (currentPos.x <= m_Left)
	{
		currentPos.x = m_Left;
		shouldUpdate = true;
	}
	else if (currentPos.x >= m_Right)
	{
		currentPos.x = m_Right;
		shouldUpdate = true;
	}

	if (currentPos.y >= m_Up)
	{
		currentPos.y = m_Up;
		shouldUpdate = true;
	}
	else if (currentPos.y <= m_Down)
	{
		currentPos.y = m_Down;
		shouldUpdate = true;
	}

	if (shouldUpdate)
		transform->Translate(currentPos);
}
