#pragma once

#include <System/SystemTemplate.h>
#include "FormationWayPoints.h"
#include "OnFormationLocked.h"

class UpdateFormationWayPoints : public SLD::SystemTemplate<FormationWayPoints>
{
public:

	//UpdateFormationWayPoints(SLD::WorldEntity& world,const rtm::float2f& leftRight,float hopTime);
	UpdateFormationWayPoints(SLD::WorldEntity& world,float hopTime);
	void OnUpdate(SLD::GameObjectId gameObjectId, float deltaTime, FormationWayPoints*) override;

private:

	void BreathingWayPoints(FormationWayPoints* points);
	void UpdateWayPoints(LineWayPoints& line);
	rtm::float2f m_LeftRightLimits;
	float m_HopInterval;
	float m_Timer;
	float m_Step;
	float m_SpreadStep;
	int m_SpreadMaxCount;
	int m_SpreadCount;
	int m_HopCount;
	int m_HopMax;
	int m_Swap;
};

