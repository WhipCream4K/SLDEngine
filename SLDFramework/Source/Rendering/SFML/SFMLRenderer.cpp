#include "SFMLRenderer.h"

#include "../../Components/RenderingComponent.h"
#include "../../Core/Window/Window.h"
#include "../../Math/rtm/vector4f.h"

void SFMLRenderer::Render(const std::vector<SLD::RenderingComponent>& renderingComponents)
{
	//m_RenderWindow.draw();

	for (auto& element : renderingComponents)
	{
		// Read Render Identifier
		const uint8_t* head{ element.GetRenderData() };
		const uint8_t* end{ head + element.GetMaxDataSize() };
		rtm::qvvf* objectWorldMatrix{};

		while (head != end)
		{
			SLD::RenderIdentifier id{ static_cast<SLD::RenderIdentifier>(*head) };
			size_t dataSize{};

			head += sizeof(SLD::RenderIdentifier);

			switch (SFMLRenderElement(id))
			{
			case SFMLRenderElement::WorldMatrix:
			{
				dataSize = sizeof(rtm::qvvf*);
				std::memcpy(&objectWorldMatrix, head, dataSize);
				break;
			}
			case SFMLRenderElement::RenderShapes:
			{
				const sf::CircleShape* circle{ reinterpret_cast<const sf::CircleShape*>(head) };
				dataSize = sizeof(sf::CircleShape);
				m_RenderWindow->draw(*circle);
				break;
			}
			case SFMLRenderElement::RenderSprite:
			{
				//sf::Sprite spriteData{static_cast<sf::Sprite>(*head)};
				const sf::Sprite* spriteData{ reinterpret_cast<const sf::Sprite*>(head) };
				dataSize = sizeof(sf::Sprite);
				
				m_RenderWindow->draw(*spriteData);
				
				break;
			}
			case SFMLRenderElement::RenderSpriteWithRenderStates:
			{
				const sf::Sprite* spriteData{ reinterpret_cast<const sf::Sprite*>(head) };
				head += sizeof(sf::Sprite);
				const sf::RenderStates* renderStates{ reinterpret_cast<const sf::RenderStates*>(head) };

				dataSize = sizeof(sf::RenderStates);
			
				m_RenderWindow->draw(*spriteData, *renderStates);

				break;
			}
			case SFMLRenderElement::RenderTexture: break;
			case SFMLRenderElement::RenderTextureWithRenderState: break;
			default: break;
			}

			head += dataSize;
		}
	}

}

void SFMLRenderer::Render(const std::vector<RefPtr<SLD::RenderingComponent>>& renderingComponents)
{
	renderingComponents;
	//renderingComponents;
}

void SFMLRenderer::Render(std::vector<SLD::RenderingComponent>& renderingComponents)
{
	for (auto& element : renderingComponents)
	{
		// Read Render Identifier
		uint8_t* head{ element.GetRenderData() };
		uint8_t* end{ head + element.GetMaxDataSize() };
		rtm::qvvf* objectWorldMatrix{};

		while (head != end)
		{
			SLD::RenderIdentifier id{ static_cast<SLD::RenderIdentifier>(*head) };
			size_t dataSize{};

			head += sizeof(SLD::RenderIdentifier);

			switch (SFMLRenderElement(id))
			{
			case SFMLRenderElement::WorldMatrix:
			{
				dataSize = sizeof(rtm::qvvf*);
				std::memcpy(&objectWorldMatrix, head, dataSize);
				break;
			}
			case SFMLRenderElement::RenderShapes:
			{
				dataSize = sizeof(sf::CircleShape);
					
				sf::CircleShape* circle{ reinterpret_cast<sf::CircleShape*>(head) };

				if (objectWorldMatrix)
				{
					sf::Vector2f scaleVec{};
					rtm::vector_store2(objectWorldMatrix->scale, &scaleVec.x);
					circle->setScale(scaleVec.x, scaleVec.y);
				}
					
				m_RenderWindow->draw(*circle);
					
				break;
			}
			case SFMLRenderElement::RenderSprite:
			{

				dataSize = sizeof(sf::Sprite);
				
				sf::Sprite* spriteData{ reinterpret_cast<sf::Sprite*>(head) };
				if(objectWorldMatrix)
				{
					sf::Vector2f scaleVec{};
					rtm::vector_store2(objectWorldMatrix->scale, &scaleVec.x);
					spriteData->setScale(scaleVec.x, scaleVec.y);
				}	

				m_RenderWindow->draw(*spriteData);

				break;
			}
			case SFMLRenderElement::RenderSpriteWithRenderStates:
			{
				const sf::Sprite* spriteData{ reinterpret_cast<const sf::Sprite*>(head) };
				head += sizeof(sf::Sprite);
				const sf::RenderStates* renderStates{ reinterpret_cast<const sf::RenderStates*>(head) };

				dataSize = sizeof(sf::RenderStates);

				m_RenderWindow->draw(*spriteData, *renderStates);

				break;
			}
			case SFMLRenderElement::RenderTexture: break;
			case SFMLRenderElement::RenderTextureWithRenderState: break;
			default: break;
			}
			
			head += dataSize;
		}
	}
}

void SFMLRenderer::SetRenderTarget(const SLD::Window& renderWindow)
{
	//sf::WindowHandle newWindowHandle{ renderWindow.GetWindowHandleToType<sf::WindowHandle>() };
	//if(m_RenderWindow->getSystemHandle() != newWindowHandle)
	//{
	//	m_RenderWindow.create(newWindowHandle);
	//}
	renderWindow;
}

void SFMLRenderer::SetRenderTarget(const RefPtr<SLD::Window>& renderWindow)
{
	auto windowSubSystem{ renderWindow->GetWindowSubsystem<SFMLWindow>()->GetSubRefPtr() };
	if (windowSubSystem != m_RenderWindow)
	{
		m_RenderWindow = windowSubSystem;
	}
	//sf::WindowHandle newWindowHandle{ renderWindow->GetWindowHandleToType<sf::WindowHandle>() };
	//if (m_RenderWindow->getSystemHandle() != newWindowHandle)
	//{
	//	//m_RenderWindow-create(newWindowHandle);
	//}
}
