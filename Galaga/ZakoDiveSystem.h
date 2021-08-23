#pragma once

#include <System/SystemTemplate.h>
#include <Physics/Components/Box2DComponent.h>
#include <Components/TransformComponent.h>
#include "ShootableComponent.h"
#include "MyComponents.h"

class ZakoDiveSystem : public SLD::SystemTemplate<
	SLD::TransformComponent,
	SLD::Box2DComponent,
	SpeedComponent,
	DiveComponent,
	EnemyTag,
	ShootableComponent,
	FormationComponent>
{
public:

	ZakoDiveSystem(SLD::WorldEntity&);

	void OnUpdate(
		SLD::GameObjectId gameObjectId, 
		float deltaTime, 
		SLD::TransformComponent*, 
		SLD::Box2DComponent*,
		SpeedComponent*, 
		DiveComponent*,
		EnemyTag*, 
		ShootableComponent*,
		FormationComponent*) override;


private:

	std::vector<rtm::float2f> m_ZakoDivePathLeft;
	std::vector<rtm::float2f> m_ZakoDivePathRight;
	std::vector<std::vector<rtm::float2f>> m_ZakoDivePath;
	SharedPtr<SLD::GameObject> m_TrailPath;
	bool m_Test{};
	//BezierPath m_ZakoDivePath{};
};

