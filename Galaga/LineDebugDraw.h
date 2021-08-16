#pragma once

#include <Rendering/SFML/SFML2DRenderSystem.h>
#include "MyComponents.h"

class LineDebugDraw : public SLD::SFML2DRenderSystem<LineComponent>
{
public:

	LineDebugDraw(SLD::WorldEntity& world);

	void OnRender(const SharedPtr<SLD::Window>& renderWindow, SLD::GameObjectId id, const SLD::TransformComponent*, LineComponent*) override;

protected:
	
	sf::RenderStates SFMLSpriteToSystemPos(
		const SharedPtr<SLD::Window>& renderWindow,
		const SLD::TransformComponent* transform) const;
};

