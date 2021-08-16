#include "Window.h"
#include "../../Miscellaneous/WinPrerequisite.h"
#include "../../Inputs/InputParams.h"

#if SLD_SUPPORT_SFML
#include "SFML/SFMLWindow.h"
#endif

#ifdef WIN32
#include <Xinput.h>
#endif

constexpr SHORT GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE{ 7849 };
constexpr SHORT GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE{ 7849 };
constexpr SHORT GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE{ 8689 };
constexpr SHORT GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE{ 8689 };
constexpr BYTE	GAMEPAD_LEFT_TRIGGER_DEADZONE{ 30 };
constexpr BYTE	GAMEPAD_RIGHT_TRIGGER_DEADZONE{ 30 };

class SLD::Window::ImplXInput
{
public:
	static constexpr uint8_t MaxSupportedController{ 4 };
	std::vector<InputParams::JoyStickEvent> GetGamepadState();
};

std::vector<SLD::InputParams::JoyStickEvent> SLD::Window::ImplXInput::GetGamepadState()
{
	using namespace InputParams;
	
	std::vector<JoyStickEvent> outState{};
	outState.reserve(MaxSupportedController);

	for (DWORD i = 0; i < DWORD(MaxSupportedController); ++i)
	{
		XINPUT_STATE state{};
		ZeroMemory(&state, sizeof(XINPUT_STATE));

		// TODO: Don't check this every frame it's really taxing
		auto result{ XInputGetState(i,&state) };
		if (result == ERROR_SUCCESS)
		{
			// success
			InputParams::JoyStickEvent joyStick{};
			joyStick.joyStickId = uint8_t(i);
			joyStick.joyStickButton = uint16_t(state.Gamepad.wButtons);

			bool isOutsideDeadZone{};

			// Left Trigger Dead zone
			isOutsideDeadZone = state.Gamepad.bLeftTrigger > GAMEPAD_LEFT_TRIGGER_DEADZONE;
			state.Gamepad.bLeftTrigger = isOutsideDeadZone ? state.Gamepad.bLeftTrigger - GAMEPAD_LEFT_TRIGGER_DEADZONE : 0;

			joyStick.leftTrigger = float(state.Gamepad.bLeftTrigger) / (UCHAR_MAX - GAMEPAD_LEFT_TRIGGER_DEADZONE);

			// Right Trigger Dead zone
			isOutsideDeadZone = state.Gamepad.bRightTrigger > GAMEPAD_RIGHT_TRIGGER_DEADZONE;
			state.Gamepad.bRightTrigger = isOutsideDeadZone ? state.Gamepad.bRightTrigger - GAMEPAD_RIGHT_TRIGGER_DEADZONE : 0;

			joyStick.rightTrigger = float(state.Gamepad.bRightTrigger) / (UCHAR_MAX - GAMEPAD_RIGHT_TRIGGER_DEADZONE);

			//gamepadState[i].leftTrigger = float(state.Gamepad.bLeftTrigger) / 255.0f;
			//gamepadState[i].rightTrigger = float(state.Gamepad.bRightTrigger) / 255.0f;

			// Left Thumb X Dead zone
			isOutsideDeadZone = state.Gamepad.sThumbLX > GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE;
			state.Gamepad.sThumbLX = isOutsideDeadZone ? state.Gamepad.sThumbLX - GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE : 0;

			joyStick.leftThumbX = float(state.Gamepad.sThumbLX) / (SHRT_MAX - GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE);

			// Left Thumb Y Dead zone
			isOutsideDeadZone = state.Gamepad.sThumbLY > GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE;
			state.Gamepad.sThumbLY = isOutsideDeadZone ? state.Gamepad.sThumbLY - GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE : 0;

			joyStick.leftThumbY = float(state.Gamepad.sThumbLY) / (SHRT_MAX - GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE);

			// Right Thumb X Dead zone
			isOutsideDeadZone = state.Gamepad.sThumbRX > GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE;
			state.Gamepad.sThumbRX = isOutsideDeadZone ? state.Gamepad.sThumbRX - GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE : 0;

			joyStick.rightThumbX = float(state.Gamepad.sThumbRX) / (SHRT_MAX - GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE);

			// Right Thumb Y Dead zone
			isOutsideDeadZone = state.Gamepad.sThumbRY > GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE;
			state.Gamepad.sThumbRY = isOutsideDeadZone ? state.Gamepad.sThumbRY - GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE : 0;

			joyStick.rightThumbY = float(state.Gamepad.sThumbRY) / (SHRT_MAX - GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE);

			outState.emplace_back(joyStick);
		}
	}

	return outState;
}

//SLD::Window::Window(uint32_t width, uint32_t height, LLWindow&& subSystemWindow, [[maybe_unused]] LLInputs&& subSystemInput,
//	const CurrentWindow& windowHandle, const std::string& vpName)
//	: m_WindowSubSystem(std::move(subSystemWindow))
//	, m_WindowEvents()
//	, m_pImplXInput(std::make_unique<ImplXInput>())
//	//, m_InputManager(std::move(subSystemInput))
//	, m_EventCntThisFrame()
//	, m_Name(vpName)
//	, m_Height(height) // test color
//	, m_Width(width)
//	, m_WindowHandle(windowHandle)
//	, m_ClearColor{ 0.0f,0.0f,0.0f,1.0f }
//	, m_ShouldVSync(true)
//{
//}

SLD::Window::Window(uint32_t width, uint32_t height, const std::string& windowTitleName)
	//: m_WindowEvents()
	: m_pImplXInput()
	//, m_EventCntThisFrame()
	, m_Name(windowTitleName)
	, m_ClearColor{}
	, m_Height(height)
	, m_Width(width)
	, m_WindowHandle()
	, m_ShouldVSync(true)
{
	InternInitialize(width, height, windowTitleName);
}

SLD::Window::~Window()
{
	InternDestroy();
}

SLD::Vector2<uint32_t> SLD::Window::GetCenter() const
{
	return Vector2<uint32_t>{m_Width / 2, m_Height / 2};
}


//void SLD::Window::Resize(uint32_t width, uint32_t height)
//{
//	auto callResize{ [&width,&height](auto& window)
//	{
//		window.Resize(width,height);
//	} };
//
//	std::visit(callResize, m_WindowSubSystem);
//
//}
//
//bool SLD::Window::PollUserInputs()
//{
//	auto callReadUserInputs{ [](auto& window)
//	{
//		return window.ReadUserInputs();
//	} };
//
//	ReadOut out{ std::visit(callReadUserInputs,m_WindowSubSystem) };
//
//	if (!out.isExit)
//	{
//		m_WindowEvents = out.buses;
//		m_EventCntThisFrame = out.eventCnt;
//
//		auto gamepadState = m_pImplXInput->GetGamepadState();
//		for (auto& state : gamepadState)
//		{
//			MessageBus bus{};
//			EventData ev{};
//
//			ev.joyStick = state;
//
//			bus.data = ev;
//			bus.eventId = uint8_t(EventType::XInputState);
//
//			m_WindowEvents[m_EventCntThisFrame++] = bus;
//		}
//	}
//
//	return out.isExit;
//}
//
//SLD::EventQueueHandle SLD::Window::GetInputData() const noexcept
//{
//	return EventQueueHandle{ m_WindowEvents,m_EventCntThisFrame };
//}
//
//void SLD::Window::Present()
//{
//	auto callPresent{ [this](auto& window)
//	{
//		window.Present(m_ShouldVSync);
//	} };
//
//	std::visit(callPresent, m_WindowSubSystem);
//}
//
//void SLD::Window::ClearBackBuffer()
//{
//	auto callClearBackBuffer{ [this](auto& window)
//	{
//		window.ClearColor(m_ClearColor);
//	} };
//
//	std::visit(callClearBackBuffer, m_WindowSubSystem);
//	
//}


