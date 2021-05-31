
#ifndef SLDFRAMEWORK_VIEWPORT_H
#define SLDFRAMEWORK_VIEWPORT_H

#include "../../Inputs/InputManager.h"

#include "MainSubSystem.h"

namespace SLD
{
	// Viewport is pretty much window instance on each platform
	// it knows the height width and can listen to user inputs
	class Window final
	{
		
	public:
		
		using CurrentWindow = std::any;
		
		Window(
			uint32_t width,
			uint32_t height,
			LLWindow&& subSystemWindow,
			LLInputs&& subSystemInput,
			const CurrentWindow& windowHandle,
			const std::string& vpName);
		
		
		//[[nodiscard]] InputManager& GetInputManager() { return m_InputManager; }

		[[nodiscard]] uint32_t GetWidth() const { return m_Width; }
		[[nodiscard]] uint32_t GetHeight() const { return m_Height; }
		[[nodiscard]] const std::string& GetViewPortName() const { return m_Name; }
		
		template<typename CastTo>
		[[nodiscard]] constexpr CastTo GetWindowHandleToType() const;

		[[nodiscard]] const std::any& GetAnyWindowHandle() const { return m_WindowHandle; }

		template<typename WindowType>
		[[nodiscard]] constexpr std::add_pointer_t<WindowType> GetWindowSubsystem();

		void Resize(uint32_t width, uint32_t height);

		// Must Use at the start of the frame
		bool PollUserInputs();
		
		EventQueueHandle GetInputData() const noexcept;

		void Present();
		void ClearBackBuffer();

	
	private:

		//bool QueryWindowEvents();
		
		// Low-Level window
		LLWindow m_WindowSubSystem;
		

		// TODO: Delete Input manager
		
		std::array<MessageBus, MinimumEventCnt> m_WindowEvents;
		uint8_t m_EventCntThisFrame;
		//InputManager m_InputManager;

		
		std::string m_Name;
		uint32_t m_Height;
		uint32_t m_Width;
		std::any m_WindowHandle;
		float m_ClearColor[4];
		bool m_ShouldVSync{};
	};

	template <typename CastTo>
	constexpr CastTo Window::GetWindowHandleToType() const
	{
		return std::any_cast<CastTo>(m_WindowHandle);
	}

	template <typename WindowType>
	constexpr std::add_pointer_t<WindowType> Window::GetWindowSubsystem()
	{
		return std::get_if<WindowType>(&m_WindowSubSystem);
	}
}



#endif
