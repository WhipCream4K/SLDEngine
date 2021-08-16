
#ifndef SLDFRAMEWORK_DX3D12RENDERER_H
#define SLDFRAMEWORK_DX3D12RENDERER_H

#include "../Core/Base.h"
//#include "../Core/Window/Window.h"
#include "../System/SystemBase.h"
//#include "LLRenderer.h"

namespace SLD
{
	class TransformComponent;
	class Window;
	class RenderingComponent;
	class Renderer final
	{
	public:

		void Render(const SharedPtr<Window>& windowToRender);
		void Render(uint8_t* data, size_t size, TransformComponent* transform);
		//void Render(const std::vector<SharedPtr<RenderingComponent>>& renderingComponents);
		//void Render(uint32_t elemCnt, uint8_t* bufferHead, size_t bufferSize);

		//void Render(const std::vector<RenderingComponent>& renderingComponents);

		//void Render(std::vector<RenderingComponent>& renderingComponents);

		//void SetRenderWindow(const SharedPtr<Window>& renderWindow);
		//void SetRenderWindow(Window& renderWindow);
	
	private:

		std::vector<SharedPtr<SystemBase>> m_RenderSystem;
		//std::vector<SharedPtr<>>
		
		//std::vector<SharedPtr<SystemBase>> m_RenderSystem;
		//LLRenderer m_SubSystemRenderer;
	};
}

#endif
