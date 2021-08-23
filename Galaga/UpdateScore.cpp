#include "UpdateScore.h"
#include "OnEnemyDead.h"

UpdateScore::UpdateScore(SLDWorldEntity& world)
	: SystemTemplate(world)
	, m_OnEnemyDead(std::make_shared<OnEnemyDead>())
{
	world.RegisterListener(m_OnEnemyDead);

	m_OnEnemyDead->Bind([this](SLD::GameObjectId id) {EnemyDead(id); });
}

void UpdateScore::EnemyDead(SLD::GameObjectId)
{
	
}