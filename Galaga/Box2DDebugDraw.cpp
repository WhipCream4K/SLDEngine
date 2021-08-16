#include "Box2DDebugDraw.h"
#include <SFML/Graphics.hpp>

#include "Core/Window/Window.h"

Box2DDebugDraw::Box2DDebugDraw(SLD::WorldEntity& world)
	: SFML2DRenderSystem(world)
{
}

void Box2DDebugDraw::OnRender(const SharedPtr<SLD::Window>& renderWindow, SLD::GameObjectId,
	const SLD::TransformComponent* transform, SLD::Box2DComponent* box2d)
{

#ifdef SLD_SUPPORT_SFML

	const auto& [width, height] {box2d->GetDimension()};
	sf::RectangleShape debugBox{ sf::Vector2f{width,height} };
	debugBox.setOutlineThickness(1.5f);
	debugBox.setFillColor(Float4ToSfColor({ 0.0f,0.0f,0.0f,0.0f }));
	debugBox.setOrigin(width / 2.0f, height / 2.0f);
	debugBox.setOutlineColor(Float4ToSfColor({ 0.0f,1.0f,0.0f,1.0f }));

	renderWindow->GetNativeWindowHandle()->draw(debugBox, SFMLSpriteToSystemPos(renderWindow, transform, { 1.0f,1.0f }));

#endif
}

sf::RenderStates Box2DDebugDraw::SFMLSpriteToSystemPos(const SharedPtr<SLD::Window>& renderWindow,
	const SLD::TransformComponent* transform, const rtm::float2f& scaleVec)
{
	const SLD::Vector2<uint32_t> viewCenter{ renderWindow->GetCenter() };
	sf::RenderStates out{};

	const auto& objMat{ transform->GetWorldMatrix() };

	sf::Vector3f posVec{};

	rtm::vector_store3(objMat.translation, &posVec.x);

	posVec.x += float(viewCenter.x);
	posVec.y *= -1;
	posVec.y += float(viewCenter.y);

	out.transform.translate(posVec.x, posVec.y);
	out.transform.scale(sf::Vector2f{ scaleVec.x,scaleVec.y });

	return out;
}
