#pragma once

#include <Rendering/SFML/SFML2DRenderSystem.h>
#include "MyComponents.h"

class TextRenderSystem : public SLD::SFML2DRenderSystem<TextRenderComponent>
{
public:

	TextRenderSystem(SLD::WorldEntity& world);
	void OnRender(const SharedPtr<SLD::Window>& renderWindow, SLD::GameObjectId id, const SLD::TransformComponent*, TextRenderComponent*) override;

private:

	sf::RenderStates SFMLTextToSystemPos(
		const SharedPtr<SLD::Window>& renderWindow,
		TextRenderComponent* textComponent,
		const SLD::TransformComponent* transform) const;
};

