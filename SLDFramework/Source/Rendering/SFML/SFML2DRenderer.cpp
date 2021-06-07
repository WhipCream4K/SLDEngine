#include "SFML2DRenderer.h"

#include "../../Components/RenderingComponent.h"
#include "../../Components/TransformComponent.h"
#include "../../Core/Window/Window.h"
#include "../../Math/rtm/vector4f.h"
#include "../../Core/Window/SFML/SFMLWindow.h"
#include "../../Core/ObservePtr.h"

class SFML2DRenderer::ImplSFML2DRenderer
{
public:

	ImplSFML2DRenderer();
	void Render(uint32_t elemCnt, uint8_t* bufferHead, size_t bufferSize);
	void SetRenderTarget(SLD::Window& renderWindow);
	void SetRenderTarget(const RefPtr<SLD::Window>& renderWindow);

private:

	struct DrawComponent
	{
		sf::Drawable* drawObj{};
		sf::RenderStates renderStates{};
		float depth{};
	};

	// TODO: Will change this to be independent when I know more about lower level of rendering
	sf::RenderWindow* m_RenderWindow;
	std::vector<DrawComponent> m_2DDrawComponent;
};


SFML2DRenderer::ImplSFML2DRenderer::ImplSFML2DRenderer()
	: m_RenderWindow(nullptr)
	, m_2DDrawComponent()
{
}

void SFML2DRenderer::ImplSFML2DRenderer::Render(uint32_t elemCnt, uint8_t* bufferHead, size_t bufferSize)
{
	auto& renderTargetCenter{ m_RenderWindow->getView().getCenter() };

	if (m_2DDrawComponent.size() < size_t(elemCnt))
		m_2DDrawComponent.resize(elemCnt);

	uint8_t* head{ bufferHead };
	uint8_t* end{ bufferHead + bufferSize };
	uint32_t cnt{};
	while (head != end)
	{
		//if (test == end)
		//	break;
		sf::RenderStates renderStates{};
		sf::Text* textEdgeCases{};
		
		// register transform
		SLD::ObservePtr<SLD::TransformComponent>* transform{};
		std::memcpy(&transform, head, sizeof(void*));

		// move
		head += sizeof(void*);

		SLD::RenderIdentifier id{ static_cast<SLD::RenderIdentifier>(*head) };

		head += sizeof(SLD::RenderIdentifier);

		sf::Drawable* drawObj{};
		size_t dataSize{};

		switch (SFMLRenderElement(id))
		{
		case SFMLRenderElement::RenderShapes: break;
		case SFMLRenderElement::RenderSprite:
		{
			dataSize = sizeof(sf::Sprite);
			drawObj = reinterpret_cast<sf::Sprite*>(head);
		}
		break;
		case SFMLRenderElement::RenderTexture: break;
		case SFMLRenderElement::RenderStates: break;
		case SFMLRenderElement::RenderText:
		{
			dataSize = sizeof(sf::Text*);
			std::memcpy(&textEdgeCases, head, sizeof(void*));
			//drawObj = reinterpret_cast<sf::Text*>(head);
		}
		break;
		default: break;
		}

		// removed since we won't know in runtime if this pointer actually a pointer
		if (transform)
		{

			auto& objMat{ transform->GetPtr()->GetWorldFinishMatrix() };
			auto& component{ m_2DDrawComponent[cnt] };

			sf::Vector3f posVec{};
			sf::Vector2f scaleVec{};

			rtm::vector_store3(objMat.translation, &posVec.x);
			rtm::vector_store2(objMat.scale, &scaleVec.x);

			posVec.x += renderTargetCenter.x;
			posVec.y *= -1;
			posVec.y += renderTargetCenter.y;

			renderStates.transform.translate(posVec.x, posVec.y);
			renderStates.transform.scale(scaleVec);

			if(textEdgeCases)
			{
				drawObj = textEdgeCases;
			}
			
			component.drawObj = drawObj;
			component.renderStates = renderStates;
			component.depth = posVec.z;

		}

		head += dataSize;
		cnt++;
	}

	// Sort Layer
	std::sort(m_2DDrawComponent.begin(), m_2DDrawComponent.end(), [](const DrawComponent& lhs, const DrawComponent& rhs)
		{
			return lhs.depth < rhs.depth;
		});

	// Draw
	for (const auto& item : m_2DDrawComponent)
	{
		// removed since we won't know in runtime if this pointer actually a pointer
		//if(item.drawObj)
		m_RenderWindow->draw(*item.drawObj, item.renderStates);
	}

}

void SFML2DRenderer::ImplSFML2DRenderer::SetRenderTarget(SLD::Window& renderWindow)
{
	auto winHandle{ renderWindow.GetWindowHandleToType<sf::WindowHandle>() };
	if (m_RenderWindow->getSystemHandle() != winHandle)
	{
		m_RenderWindow = &renderWindow.GetWindowSubSystem<SFMLWindow>()->GetSFMLWindow();
	}
}

void SFML2DRenderer::ImplSFML2DRenderer::SetRenderTarget(const RefPtr<SLD::Window>& renderWindow)
{
	if (renderWindow)
	{
		auto winHandle{ renderWindow->GetWindowHandleToType<sf::WindowHandle>() };
		if (!m_RenderWindow ||
			m_RenderWindow->getSystemHandle() != winHandle)
		{
			m_RenderWindow = &renderWindow->GetWindowSubSystem<SFMLWindow>()->GetSFMLWindow();
		}
	}
}


SFML2DRenderer::SFML2DRenderer()
	: m_pImplRenderWindow(std::make_unique<ImplSFML2DRenderer>())
{
}

SFML2DRenderer::~SFML2DRenderer() = default;

void SFML2DRenderer::Render(uint32_t elemCnt, uint8_t * bufferHead, size_t bufferSize)
{
	m_pImplRenderWindow->Render(elemCnt, bufferHead, bufferSize);
}

void SFML2DRenderer::SetRenderTarget(SLD::Window & renderWindow)
{
	m_pImplRenderWindow->SetRenderTarget(renderWindow);
}

void SFML2DRenderer::SetRenderTarget(const RefPtr<SLD::Window>&renderWindow)
{
	m_pImplRenderWindow->SetRenderTarget(renderWindow);
}
