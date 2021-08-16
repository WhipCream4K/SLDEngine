#pragma once

#include <Rendering/SFML/SFML2DRenderSystem.h>
#include <Physics/Components/Box2DComponent.h>
//#include <SFML/Graphics/RenderStates.hpp>

class Box2DDebugDraw : public SLD::SFML2DRenderSystem<SLD::Box2DComponent>
{
public:

	Box2DDebugDraw(SLD::WorldEntity& world);
	void OnRender(const SharedPtr<SLD::Window>& renderWindow, SLD::GameObjectId id, const SLD::TransformComponent*, SLD::Box2DComponent*) override;

private:

	sf::RenderStates SFMLSpriteToSystemPos(
		const SharedPtr<SLD::Window>& renderWindow,
		const SLD::TransformComponent* transform,
		const rtm::float2f& scaleVec);
};

