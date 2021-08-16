#include "PlayerInputSystem.h"
#include "Components/InputComponent.h"
#include "BulletManager.h"
#include "ShootableComponent.h"

PlayerInputSystem::PlayerInputSystem(SLD::WorldEntity& world)
	: SystemTemplate(world,SLD::PipelineLayer::OnFixedUpdate)
{
}

void PlayerInputSystem::OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, SLD::TransformComponent* transform,
	InputListener*, SLD::Box2DComponent* box2d, SpeedComponent* speed)
{
	using namespace SLD;

	SLD::InputSettings& inputSettings{ m_World.GetWorldInputSetting() };

	float axisX{ inputSettings.GetInputAxis("Horizontal") };
	float axisY{ inputSettings.GetInputAxis("Vertical") };

	if (inputSettings.GetInputState("Shoot") == InputEvent::IE_Released)
	{
		std::cout << "Shoot : " << m_FrameCounter++ << '\n';
		
		auto barrelPoint{ transform->GetWorldPos() };
		barrelPoint.y += box2d->GetDimension().y;

		ShootableComponent* shooter{ m_World.GetComponent<ShootableComponent>(gameObjectId) };
		if (shooter)
			shooter->Shoot(m_World, barrelPoint,{0.0f,1.0f});
	}

	box2d->GetBody()->SetLinearVelocity({ speed->value * deltaTime * axisX,speed->value * deltaTime * axisY });
}
