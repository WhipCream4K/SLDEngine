#pragma once

#include <System/SystemTemplate.h>
#include <System/AsyncSystemT.h>
#include <Physics/Components/Box2DComponent.h>
#include "MyComponents.h"
#include "EnemyPath.h"

class FormationWayPoints;
class EnemyState;
class EnemyStateSystem :
	public SLD::SystemTemplate<
	SLD::TransformComponent,
	SLD::Box2DComponent,
	SpeedComponent,
	FlyInComponent,
	FormationComponent,
	EnemyTag>
{
public:

	EnemyStateSystem(SLD::WorldEntity& world);
	EnemyStateSystem(SLD::WorldEntity& world, SLD::GameObjectId pathId);
	
	void OnUpdate(
		SLD::GameObjectId gameObjectId, 
		float deltaTime, 
		SLD::TransformComponent*, 
		SLD::Box2DComponent*, 
		SpeedComponent*, 
		FlyInComponent*, 
		FormationComponent*, 
		EnemyTag*) override;


private:

	void HandleFlyInState(
		float dt,
		SLD::TransformComponent* transform,
		SLD::Box2DComponent* box2d,
		FlyInComponent* flyIn,
		SpeedComponent* speed,
		EnemyTag* tag);

	void HandleFormationState(
		float dt,
		SLD::TransformComponent* transform,
		SLD::Box2DComponent* box2d,
		FormationComponent* formation,
		SpeedComponent* speed,
		EnemyTag* tag);

	//void HandleDiveState(
	//	float dt,
	//	SLD::TransformComponent* transform,
	//	SLD::Box2DComponent* box2d,
	//	FormationComponent* formation,
	//	SpeedComponent* speed,
	//	EnemyTag* tag);

	
	// Position in formation
	std::vector<std::vector<rtm::float2f>> m_LineFormationWayPoints;
	SLD::GameObjectId m_FormationWayPointObjectId;
	std::future<int> m_FutureWayPoint;
};

