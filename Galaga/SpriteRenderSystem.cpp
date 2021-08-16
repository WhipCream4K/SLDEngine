#include "SpriteRenderSystem.h"

#include "Core/Window/Window.h"

#include "ResourceManager.h"

SpriteRenderSystem::SpriteRenderSystem(SLD::WorldEntity& world)
	: SFML2DRenderSystem(world)
{
}

void SpriteRenderSystem::OnRender(const SharedPtr<SLD::Window>& renderWindow, SLD::GameObjectId,
                                  const SLD::TransformComponent* transform, SpriteRenderComponent* sprite)
{
	using namespace SLD;
	
#ifdef SLD_SUPPORT_SFML

	if(sprite->shouldRender)
		renderWindow->GetNativeWindowHandle()->draw(sprite->sprite,SFMLSpriteToSystemPos(renderWindow,sprite,transform));
	
#endif
}

sf::RenderStates SpriteRenderSystem::SFMLSpriteToSystemPos(const SharedPtr<SLD::Window>& renderWindow,
	SpriteRenderComponent* spriteComponent, const SLD::TransformComponent* transform) const
{
	const SLD::Vector2<uint32_t> viewCenter{ renderWindow->GetCenter() };

	[[maybe_unused ]]const auto& textRect{ spriteComponent->sprite.getTextureRect() };
	float spriteWidth{ float(textRect.width) };
	float spriteHeight{ float(textRect.height) };

	spriteComponent->sprite.setOrigin(spriteWidth * spriteComponent->spriteOffset.x, spriteHeight * spriteComponent->spriteOffset.y);
	spriteComponent->sprite.setScale(spriteComponent->scaleVec.x,spriteComponent->scaleVec.y);
	
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