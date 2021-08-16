#include "LineDebugDraw.h"
#include <Core/Window/Window.h>
#include <SFML/Graphics.hpp>

LineDebugDraw::LineDebugDraw(SLD::WorldEntity& world)
	: SFML2DRenderSystem(world)
{
}

void LineDebugDraw::OnRender(const SharedPtr<SLD::Window>& renderWindow, SLD::GameObjectId,
                             const SLD::TransformComponent* transform, LineComponent* line)
{

#ifdef SLD_SUPPORT_SFML

	sf::VertexArray vertexArraty{};
	vertexArraty.setPrimitiveType(sf::LinesStrip);
	for (const auto& p  : line->points)
	{
		vertexArraty.append(sf::Vertex{ {p.x,-p.y} });
	}

	renderWindow->GetNativeWindowHandle()->draw(&vertexArraty[0], vertexArraty.getVertexCount(), sf::LineStrip, SFMLSpriteToSystemPos(renderWindow, transform));
	
#endif
	
}

sf::RenderStates LineDebugDraw::SFMLSpriteToSystemPos(const SharedPtr<SLD::Window>& renderWindow,
	const SLD::TransformComponent* transform) const
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

	return out;
}
