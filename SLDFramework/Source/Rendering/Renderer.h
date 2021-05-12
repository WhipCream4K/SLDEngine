
#ifndef SLDFRAMEWORK_DX3D12RENDERER_H
#define SLDFRAMEWORK_DX3D12RENDERER_H

#include "../Core/Base.h"

#include "LLRenderer.h"

namespace SLD
{
	class Window;
	class RenderingComponent;
	class Renderer final
	{
	public:

		void Render(const std::vector<RefPtr<RenderingComponent>>& renderingComponents);

		void Render(const std::vector<RenderingComponent>& renderingComponents);

		void SetRenderWindow(const RefPtr<Window>& renderWindow);
		void SetRenderWindow(const Window& renderWindow);
	
	private:

		LLRenderer m_SubSystemRenderer;
	};
}

#endif
