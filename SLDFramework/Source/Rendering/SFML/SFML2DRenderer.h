
#ifndef SLDFRAMEWORK_SFMLRENDERER_H
#define SLDFRAMEWORK_SFMLRENDERER_H

#include "../../Core/Base.h"

namespace SLD
{
	class RenderingComponent;
	class Window;
}


class SFML2DRenderer final
{
public:

	class ImplSFML2DRenderer;
	SFML2DRenderer();
	~SFML2DRenderer();
	void Render(const std::vector<SLD::RenderingComponent>& renderingComponents);
	void Render(const std::vector<RefPtr<SLD::RenderingComponent>>& renderingComponents);
	void Render(uint32_t elemCnt,uint8_t* bufferHead, size_t bufferSize);

	void SetRenderTarget(SLD::Window& renderWindow);
	void SetRenderTarget(const RefPtr<SLD::Window>& renderWindow);

private:

	OwnedPtr<ImplSFML2DRenderer> m_pImplRenderWindow;

	//struct DrawComponent
	//{
	//	const sf::Drawable* drawObj{};
	//	sf::RenderStates renderStates{};
	//	float depth{};
	//};

	//std::vector<DrawComponent> m_SFMLDrawable;
};

#endif


