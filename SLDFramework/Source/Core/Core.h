

#ifndef SLDFRAMEWORK_CORE_H
#define SLDFRAMEWORK_CORE_H

#include "Base.h"
#include "../Miscellaneous/WinPrerequisite.h"

#include "../Debugging/GUIDebugComponent.h"
#include "../Rendering/Renderer.h"
#include "WorldEntity.h"

#define CAN_CREATE_WINDOW_HANDLE 

#ifdef SLDFRAMEWORK_PROVIDE_WINDOW_HANDLE
#undef CAN_CREATE_WINDOW_HANDLE 
#endif

namespace SLD
{
	class Core final // singleton
	{
	public:

		Core();

		bool Init();

		using windowHandle = std::any;
		bool CreateViewPortFromHWND(const windowHandle& windowHandle);

#ifdef CAN_CREATE_WINDOW_HANDLE
		bool CreateNewViewPort(uint32_t width, uint32_t height,const std::string& winName);
#endif

#pragma region Big 5

		bool TranslateUserInputs();
		void Step();
		WorldEntity& GetDefaultWorldEntity();
		
		Core(const Core&) = delete;
		Core& operator=(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(Core&&) = delete;
	
		~Core();
		
#pragma endregion
	
	private:

		// TODO: possible implementation; make multiple windows
		RefPtr<Window> m_MainViewPort;
		GUIDebugComponent m_GuiDebugger;
		Renderer m_MainRenderer;
		WorldEntity m_WorldEntity;

		void ShouldResizeWindow(const RefPtr<Window>& window, const EventQueueHandle& userEv);
		
#if defined(_WIN32) && defined(CAN_CREATE_WINDOW_HANDLE)
		WNDCLASSEX m_WindowClass{};
		HWND m_WindowHandle{};
		bool m_DoesFrameworkOwnWindowHandle{};
#endif

		// For now this flag will prevent user from creating multiple viewports
		bool m_AlreadyHaveViewPort{};
	};
}


#endif
