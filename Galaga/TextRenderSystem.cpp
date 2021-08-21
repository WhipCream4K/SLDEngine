#include "TextRenderSystem.h"
#include <Core/Window/Window.h>

TextRenderSystem::TextRenderSystem(SLD::WorldEntity& world)
	: SFML2DRenderSystem(world)
{
}

void TextRenderSystem::OnRender(const SharedPtr<SLD::Window>& renderWindow, SLD::GameObjectId,
                                const SLD::TransformComponent* transform, TextRenderComponent* textComponent)
{
	
#ifdef SLD_SUPPORT_SFML
	
	sf::Text some{ textComponent->text.c_str(),*textComponent->font,unsigned(textComponent->fontSize) };
	renderWindow->GetNativeWindowHandle()->draw(some, SFMLTextToSystemPos(renderWindow, &some, transform));
	
#endif
}

sf::RenderStates TextRenderSystem::SFMLTextToSystemPos(const SharedPtr<SLD::Window>& renderWindow,
	sf::Text* textComponent, const SLD::TransformComponent* transform) const
{
	textComponent;
	const SLD::Vector2<uint32_t> viewCenter{ renderWindow->GetCenter() };

	//const size_t textSize{ textComponent->text.getCharacterSize() };
	//const auto& textOrigin{ textComponent->text.getOrigin() };
	//textComponent->text.setOrigin(float(textSize),textOrigin.y);


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

