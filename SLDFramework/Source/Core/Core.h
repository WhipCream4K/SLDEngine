#ifndef SLDFRAMEWORK_CORE_H
#define SLDFRAMEWORK_CORE_H

#include "Base.h"
#include "../Miscellaneous/WinPrerequisite.h"

#include "../Debugging/GUIDebugComponent.h"
#include "../Rendering/Renderer.h"
#include "Window/Window.h"
#include "WorldEntity.h"
#include "Clock/WorldClock.h"

#define CAN_CREATE_WINDOW_HANDLE 

#ifdef SLDFRAMEWORK_PROVIDE_WINDOW_HANDLE
#undef CAN_CREATE_WINDOW_HANDLE 
#endif

namespace SLD
{
	class Core final
	{
	public:

		Core();

		bool Init();

		using windowHandle = std::any;
		bool CreateViewPortFromHWND(const windowHandle& windowHandle);

		bool CreateWindowFrame(uint32_t width, uint32_t height, const std::string& titleName);
		
#ifdef CAN_CREATE_WINDOW_HANDLE
		bool CreateNewViewPort(uint32_t width, uint32_t height,const std::string& winName);
#endif

		bool TranslateUserInputs();
		void Step();
		WorldEntity& GetDefaultWorldEntity();
		WorldClock& GetClock();
		
#pragma region Big 5

		Core(const Core&) = delete;
		Core& operator=(const Core&) = delete;
		Core(Core&&) = delete;
		Core& operator=(Core&&) = delete;
	
		~Core();
		
#pragma endregion
	
	private:

		// TODO: possible implementation; make multiple windows
		SharedPtr<Window> m_MainViewPort;
		GUIDebugComponent m_GuiDebugger;
		Renderer m_MainRenderer;
		WorldEntity m_WorldEntity;
		WorldClock m_WorldClock;

		//void ShouldResizeWindow(const SharedPtr<Window>& window, const EventQueueHandle& userEv);
		
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
