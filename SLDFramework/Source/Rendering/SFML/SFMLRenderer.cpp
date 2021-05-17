#include "SFMLRenderer.h"

#include "../../Components/RenderingComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Core/Window/Window.h"
#include "../../Math/rtm/vector4f.h"
#include "../../Core/ObservePtr.h"

void SFMLRenderer::Render(const std::vector<SLD::RenderingComponent>& renderingComponents)
{
	//m_RenderWindow.draw();
	auto& renderTargetCenter{ m_RenderWindow->getView().getCenter() };

	for (const auto& element : renderingComponents)
	{
		// Read Render Identifier
		const uint8_t* head{ element.GetRenderData() };
		const uint8_t* end{ head + element.GetMaxDataSize() };
		//const rtm::qvvf* objectWorldMatrix{};
		const SLD::ObservePtr<SLD::TransformComponent>* transform{};
		const sf::Drawable* sfmlDrawable{};
		sf::RenderStates renderStates{ sf::RenderStates::Default };

		while (head != end)
		{
			SLD::RenderIdentifier id{ static_cast<SLD::RenderIdentifier>(*head) };
			size_t dataSize{};

			head += sizeof(SLD::RenderIdentifier);

			switch (SFMLRenderElement(id))
			{
			case SFMLRenderElement::WorldMatrix:
			{
				dataSize = sizeof(void*);
				std::memcpy(&transform, head, dataSize);
				break;
			}
			case SFMLRenderElement::RenderShapes:
			{
				dataSize = sizeof(sf::CircleShape);
				sfmlDrawable = reinterpret_cast<const sf::CircleShape*>(head);
				break;
			}
			case SFMLRenderElement::RenderSprite:
			{
				dataSize = sizeof(sf::Sprite);
				sfmlDrawable = reinterpret_cast<const sf::Sprite*>(head);
				break;
			}
			case SFMLRenderElement::RenderStates:
			{
				dataSize = sizeof(sf::RenderStates);
				renderStates = *reinterpret_cast<const sf::RenderStates*>(head);
				break;
			}
			case SFMLRenderElement::RenderTexture: break;
			default: break;
			}

			// SFML Drawable
			if (sfmlDrawable)
			{
				if (transform)
				{
					// TODO: Do rotation
					
					sf::Vector2f posVec{};
					sf::Vector2f scaleVec{};
					auto& objMat{ transform->GetPtr()->GetWorldFinishMatrix() };
					rtm::vector_store2(objMat.translation, &posVec.x);
					rtm::vector_store2(objMat.scale, &scaleVec.x);

					posVec.x += renderTargetCenter.x;
					posVec.y *= -1.0f;
					posVec.y += renderTargetCenter.y;

					renderStates.transform.translate(posVec);
					renderStates.transform.scale(scaleVec);
					//renderStates.transform.rotate()
				}

				m_RenderWindow->draw(*sfmlDrawable,renderStates);
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
	auto& renderTargetCenter{ m_RenderWindow->getView().getCenter() };

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

			sf::Drawable* sfmlDrawable{};
			sf::RenderStates renderStates{ sf::RenderStates::Default };
			//sf::FloatRect bounding{};

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
				sfmlDrawable = reinterpret_cast<sf::CircleShape*>(head);
				break;
			}
			case SFMLRenderElement::RenderSprite:
			{
				dataSize = sizeof(sf::Sprite);
				sfmlDrawable = reinterpret_cast<sf::Sprite*>(head);
				break;
			}
			case SFMLRenderElement::RenderTexture: break;
			case SFMLRenderElement::RenderStates: break;
			default: break;
			}

			// SFML Drawable
			if (sfmlDrawable)
			{
				if (objectWorldMatrix)
				{
					sf::Vector2f posVec{};
					rtm::vector_store2(objectWorldMatrix->translation, &posVec.x);

					posVec.x += renderTargetCenter.x;
					posVec.y += renderTargetCenter.y;

					renderStates.transform.translate(posVec);
				}

				m_RenderWindow->draw(*sfmlDrawable, renderStates);
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
