#include "InputManager.h"

#include "InputSettings.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "../Miscellaneous/WinPrerequisite.h"
#include "../Miscellaneous/WinXInput.h"
#include "../Core/Window/Window.h"

constexpr SHORT GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE{ 7849 };
constexpr SHORT GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE{ 7849 };
constexpr SHORT GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE{ 8689 };
constexpr SHORT GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE{ 8689 };
constexpr BYTE	GAMEPAD_LEFT_TRIGGER_DEADZONE{ 30 };
constexpr BYTE	GAMEPAD_RIGHT_TRIGGER_DEADZONE{ 30 };

std::array<SLD::InputParams::JoyStickEvent,
	SLD::InputManager::MaxSupportedControllers> SLD::InputManager::m_MainJoyStickEvents{};

uint32_t SLD::InputManager::CreateHashId(const std::string& str, InputEvent ie)
{
	// This function is from https://en.cppreference.com/w/cpp/utility/hash

	const uint32_t hashName = uint32_t(std::hash<std::string>{}(str));
	const uint32_t hashIE = uint32_t(std::hash<InputEvent>{}(ie));
	return hashName ^ (hashIE << 1);
}

void SLD::InputManager::QueryUserInputsAsyncMaybe([[maybe_unused]] const SharedPtr<Window>& relativeWindow, KeyPool& keyPool)
{
	using namespace InputParams;

#ifdef SLD_SUPPORT_SFML

	const uint8_t activeGamepadCnt{ PollXInputJoyStickEvent() };

	for (auto& [key, state] : keyPool)
	{
		bool isPressed{};
		float& valueFromDevice{ state.axisValue };
		uint8_t& userIndex{ state.userIndex };

		switch (key.inputDevice)
		{
		case InputDevice::D_Keyboard:

			isPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Key(key.keyCode));
			valueFromDevice = isPressed ? 1.0f : 0.0f;

			// keyboard only returned player index 0, for now
			userIndex = 0;

			break;
		case InputDevice::D_Mouse: break;
		case InputDevice::D_Gamepad:

			for (uint8_t i = 0; i < activeGamepadCnt; ++i)
			{
				const auto& stickEvents{ m_MainJoyStickEvents[i] };

				// process the float value of the Game pad
				switch (GamePadKey(key.keyCode))
				{
				case GamePadKey::GPK_Left_Shoulder:		
					valueFromDevice = stickEvents.leftTrigger;
					isPressed = stickEvents.axisValueTriggered;
					break;
				case GamePadKey::GPK_Right_Shoulder:	
					valueFromDevice = stickEvents.rightTrigger;
					isPressed = stickEvents.axisValueTriggered;
					break;
				case GamePadKey::GPK_Right_AxisX:		
					valueFromDevice = stickEvents.rightThumbX;
					isPressed = stickEvents.axisValueTriggered;
					break;
				case GamePadKey::GPK_Right_AxisY:		
					valueFromDevice = stickEvents.rightThumbY;
					isPressed = stickEvents.axisValueTriggered;
					break;
				case GamePadKey::GPK_Left_AxisX:		
					valueFromDevice = stickEvents.leftThumbX;
					isPressed = stickEvents.axisValueTriggered;
					break;
				case GamePadKey::GPK_Left_AxisY:		
					valueFromDevice = stickEvents.leftThumbY;
					isPressed = stickEvents.axisValueTriggered;
					break;
				default:
					isPressed = stickEvents.joyStickButton & key.keyCode;
					break;
				}

				userIndex = stickEvents.joyStickId;
			}

			break;
		}

		// Change Input Event
		InputEvent& currentKeyState{ state.keyEvent };

		if (isPressed)
		{
			switch (currentKeyState)
			{
			case InputEvent::IE_Pressed:	currentKeyState = InputEvent::IE_Down;		break;
			case InputEvent::IE_Released:
			case InputEvent::IE_None:		currentKeyState = InputEvent::IE_Pressed;	break;
			default: break;
			}
		}
		else
		{
			switch (currentKeyState)
			{
			case InputEvent::IE_Pressed:
			case InputEvent::IE_Down:		currentKeyState = InputEvent::IE_Released; break;
			case InputEvent::IE_Released:	currentKeyState = InputEvent::IE_None;		break;
			default: break;
			}
		}
	}

#endif
}

uint8_t SLD::InputManager::PollXInputJoyStickEvent()
{
	uint8_t activeCnt{};

#ifdef _WIN32

	for (DWORD i = 0; i < DWORD(MaxSupportedControllers); ++i)
	{
		XINPUT_STATE state{};
		auto result{ XInputGetState(i,&state) };
		if (result == ERROR_SUCCESS)
		{
			// success
			InputParams::JoyStickEvent joyStick{};
			joyStick.joyStickId = uint8_t(i);
			joyStick.joyStickButton = uint16_t(state.Gamepad.wButtons);

			bool isOutsideDeadZone{ true };
			uint8_t isAxisTriggeredCnt{};
			uint8_t isAxisTriggered{};

			// Left Trigger Dead zone
			isOutsideDeadZone = state.Gamepad.bLeftTrigger > GAMEPAD_LEFT_TRIGGER_DEADZONE;
			isAxisTriggered += uint8_t((isOutsideDeadZone) << isAxisTriggeredCnt++);
			state.Gamepad.bLeftTrigger = isOutsideDeadZone ? state.Gamepad.bLeftTrigger - GAMEPAD_LEFT_TRIGGER_DEADZONE : 0;

			joyStick.leftTrigger = float(state.Gamepad.bLeftTrigger) / (UCHAR_MAX - GAMEPAD_LEFT_TRIGGER_DEADZONE);

			// Right Trigger Dead zone
			isOutsideDeadZone = state.Gamepad.bRightTrigger > GAMEPAD_RIGHT_TRIGGER_DEADZONE;
			isAxisTriggered = uint8_t(isOutsideDeadZone << isAxisTriggeredCnt++);
			state.Gamepad.bRightTrigger = isOutsideDeadZone ? state.Gamepad.bRightTrigger - GAMEPAD_RIGHT_TRIGGER_DEADZONE : 0;

			joyStick.rightTrigger = float(state.Gamepad.bRightTrigger) / (UCHAR_MAX - GAMEPAD_RIGHT_TRIGGER_DEADZONE);

			// Left Thumb X Dead zone
			SHORT& leftThumbX{ state.Gamepad.sThumbLX };
			float sign{ leftThumbX < 0 ? -1.0f : 1.0f };
			const int absLeftThumb{ abs(int(leftThumbX)) };
			isOutsideDeadZone = absLeftThumb > GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE;
			isAxisTriggered += uint8_t(isOutsideDeadZone << isAxisTriggeredCnt++);
			leftThumbX = (isOutsideDeadZone ? (SHORT)(absLeftThumb)-GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE : 0);

			joyStick.leftThumbX = float(leftThumbX) * sign / (SHRT_MAX - GAMEPAD_LEFT_THUMBSTICK_X_DEADZONE);

			// Left Thumb Y Dead zone
			SHORT& leftThumbY{ state.Gamepad.sThumbLY };
			const int absLeftThumbY{ abs(int(leftThumbY)) };
			sign = leftThumbY < 0 ? -1.0f : 1.0f;
			isOutsideDeadZone = absLeftThumbY > GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE;
			isAxisTriggered += uint8_t(isOutsideDeadZone << isAxisTriggeredCnt++);
			leftThumbY = isOutsideDeadZone ? (SHORT)absLeftThumbY - GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE : 0;

			joyStick.leftThumbY = float(leftThumbY) * sign / (SHRT_MAX - GAMEPAD_LEFT_THUMBSTICK_Y_DEADZONE);

			// Right Thumb X Dead zone
			SHORT& rightThumbX{ state.Gamepad.sThumbRX };
			sign = rightThumbX < 0 ? -1.0f : 1.0f;
			const int absRightThumbX{ abs(int(rightThumbX)) };
			isOutsideDeadZone = absRightThumbX > GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE;
			isAxisTriggered += uint8_t(isOutsideDeadZone << isAxisTriggeredCnt++);
			rightThumbX = isOutsideDeadZone ? (SHORT)absRightThumbX - GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE : 0;

			joyStick.rightThumbX = float(rightThumbX) * sign / (SHRT_MAX - GAMEPAD_RIGHT_THUMBSTICK_X_DEADZONE);

			// Right Thumb Y Dead zone
			SHORT& rightThumbY{ state.Gamepad.sThumbRY };
			sign = rightThumbX < 0 ? -1.0f : 1.0f;
			const int absRightThumbY{ abs(int(rightThumbX)) };
			isOutsideDeadZone = absRightThumbY > GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE;
			isAxisTriggered += uint8_t(isOutsideDeadZone << isAxisTriggeredCnt++);
			rightThumbY = isOutsideDeadZone ? (SHORT)absRightThumbY - GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE : 0;

			joyStick.rightThumbY = float(rightThumbY) * sign / (SHRT_MAX - GAMEPAD_RIGHT_THUMBSTICK_Y_DEADZONE);

			joyStick.axisValueTriggered = isAxisTriggered ^ 0;

			m_MainJoyStickEvents[activeCnt] = joyStick;
			++activeCnt;
		}
	}

#endif

	return activeCnt;
}