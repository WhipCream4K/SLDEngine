
#ifndef SLDFRAMEWORK_SFMLRENDERER_H
#define SLDFRAMEWORK_SFMLRENDERER_H

#include "../../Miscellaneous/SFMLPrerequisite.h"
#include "../../Core/Base.h"

namespace SLD
{
	class RenderingComponent;
	class Window;
}

#ifdef HAS_SFML

class SFMLRenderer final
{
public:
	
	void Render(const std::vector<SLD::RenderingComponent>& renderingComponents);
	void Render(const std::vector<RefPtr<SLD::RenderingComponent>>& renderingComponents);
	void Render(std::vector<SLD::RenderingComponent>& renderingComponents);

	void SetRenderTarget(const SLD::Window& renderWindow);
	void SetRenderTarget(const RefPtr<SLD::Window>& renderWindow);

private:
	
	RefPtr<sf::RenderWindow> m_RenderWindow;
};

#endif


#endif
