#include "UpdateProjectile.h"
#include "BulletManager.h"

UpdateProjectile::UpdateProjectile(SLD::WorldEntity& world, float upLimit)
	: SystemTemplate(world, SLD::PipelineLayer::OnPreRender)
	, m_UpLimit(upLimit)
{
}

void UpdateProjectile::OnUpdate(SLD::GameObjectId, float , SLD::TransformComponent* transform,
	ProjectileComponent* projectile, SLD::Box2DComponent* box2D)
{
	const auto& worldPos{ transform->GetWorldPos() };

	if(projectile->isActive)
	{
		if (worldPos.y > m_UpLimit)
		{
			projectile->isActive = false;
		}
	}
	else
	{
		const auto& physicsBody{ box2D->GetBody() };
		physicsBody->SetTransform({ 0.0f,1000.0f }, 0.0f);
		physicsBody->SetAwake(false);
	}
}
