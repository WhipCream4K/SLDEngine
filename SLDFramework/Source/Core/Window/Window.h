
#ifndef SLDFRAMEWORK_VIEWPORT_H
#define SLDFRAMEWORK_VIEWPORT_H

#include "../../Inputs/InputManager.h"

//#include "MainSubSystem.h"
namespace sf
{
	class RenderWindow;
}

namespace SLD
{

#if SLD_SUPPORT_SFML
	using WindowHandle = SharedPtr<sf::RenderWindow>;
#else
	using WindowHandle = void*;
#endif

	class Window
	{

	public:

		struct WindowMessages
		{
			std::array<InputParams::MessageBus2, MinimumEventCnt> wndMessages;
			uint8_t totalMessages = 0;
		};

		class ImplXInput;
		using CurrentWindow = std::any;

		Window(uint32_t width, uint32_t height, const std::string& windowTitleName);
		virtual ~Window();

		[[nodiscard]] uint32_t GetWidth() const { return m_Width; }
		[[nodiscard]] uint32_t GetHeight() const { return m_Height; }
		[[nodiscard]] Vector2<uint32_t> GetCenter() const;
		[[nodiscard]] const std::string& GetTitleName() const { return m_Name; }

		// Discard the use of std any because it's not good for run-time
		[[nodiscard]] WindowHandle GetNativeWindowHandle() const { return m_WindowHandle; }

		// Must Use at the start of the frame
		//bool PollUserInputs();

		//EventQueueHandle GetInputData() const noexcept;
		const WindowMessages& PollWindowMessages(bool& shouldEarlyExit);

		virtual void OnResize(uint32_t, uint32_t) {}
		virtual void OnGainFocus() {}
		virtual void OnLostFocus() {}
		virtual void OnWindowMove() {}

		//TODO: Maybe send out a notification that any entities can subscribe on
		void Resize(uint32_t width, uint32_t height);
		void SetVSync(bool value);
		void Present();
		void ClearBackBuffer();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;

	private:

		void InternInitialize(uint32_t width, uint32_t height, const std::string& windowTitleName);
		void InternDestroy() noexcept;

		WindowMessages m_WndMessages;
		OwnedPtr<ImplXInput> m_pImplXInput;

		std::string m_Name;
		float m_ClearColor[4];
		uint32_t m_Height;
		uint32_t m_Width;
		WindowHandle m_WindowHandle;
		bool m_ShouldVSync{};
	};

	//template <typename CastTo>
	//constexpr CastTo Window::GetWindowHandleToType() const
	//{
	//	return std::any_cast<CastTo>(m_WindowHandle);
	//}

	//template <typename WindowType>
	//constexpr std::add_pointer_t<WindowType> Window::GetWindowSubSystem() noexcept
	//{
	//	return std::get_if<WindowType>(&m_WindowSubSystem);
	//}
}


#endif
