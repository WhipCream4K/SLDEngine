#pragma once

#include <Rendering/SFML/SFML2DRenderSystem.h>
#include "MyComponents.h"

class SpriteRenderSystem : public SLD::SFML2DRenderSystem<SpriteRenderComponent>
{
public:

	SpriteRenderSystem(SLD::WorldEntity& world);
	void OnRender(const SharedPtr<SLD::Window>& renderWindow, SLD::GameObjectId id, const SLD::TransformComponent*, SpriteRenderComponent*) override;
	virtual ~SpriteRenderSystem() = default;

protected:

	sf::RenderStates SFMLSpriteToSystemPos(
		const SharedPtr<SLD::Window>& renderWindow,
		SpriteRenderComponent* spriteComponent,
		const SLD::TransformComponent* transform) const;
	
};

