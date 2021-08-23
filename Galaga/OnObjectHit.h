#pragma once

#include <SLDFramework.h>

class OnObjectHit
{
public:

	OnObjectHit() = default;
	virtual ~OnObjectHit() = default;
	virtual void OnHit(SLD::WorldEntity& ,SLD::GameObjectId,bool) {}
};

class PlayerOnHit : public OnObjectHit
{
public:

	void OnHit(SLD::WorldEntity&, SLD::GameObjectId,bool isDead) override;
};

class EnemyHit : public OnObjectHit
{
public:

	void OnHit(SLD::WorldEntity&, SLD::GameObjectId, bool) override;
};

class BossHit : public EnemyHit
{
public:

	void OnHit(SLD::WorldEntity&, SLD::GameObjectId, bool) override;
};