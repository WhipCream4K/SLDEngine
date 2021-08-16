

#ifndef SLDFRAMEWORK_INPUTMANAGER_H
#define SLDFRAMEWORK_INPUTMANAGER_H

#include "../Core/Base.h"
#include "InputParams.h"
#include "Key.h"

namespace SLD
{	
	//struct EventQueueHandle
	//{
	//	EventQueueHandle(const std::array<MessageBus, MinimumEventCnt>& ev,const uint8_t& evCnt)
	//		: events(ev)
	//		, eventCntThisFrame(evCnt)
	//	{
	//	}
	//	
	//	const std::array<MessageBus, MinimumEventCnt>& events;
	//	const uint8_t& eventCntThisFrame;
	//};

	// SFML interface for this one
	// NOTE: I couldn't explicitly type all the key codes just for the sake of originality
	class Window;
	class InputManager final
	{
	public:

		using CurrentWindow = std::any;

		static uint32_t CreateHashId(const std::string& str, InputEvent ie);

		//InputManager(const CurrentWindow& handle);
		InputManager();

		/// <summary>
		/// From window API translate all the messages came through from the user
		/// and execute commands registered within current input setting
		/// </summary>
		/// <returns>return true if window exits</returns>
		bool TranslateWindowsMessages();

		static void QueryUserInputsAsyncMaybe(
			const SharedPtr<Window>& relativeWindow,
			KeyPool& keyPool);
		
		static uint8_t PollXInputJoyStickEvent();
		
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;

		InputManager(InputManager&& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		~InputManager() = default;
		
		static constexpr uint8_t MaxSupportedControllers{ 4 };

	private:

		static std::array<InputParams::JoyStickEvent,
		MaxSupportedControllers> m_MainJoyStickEvents;
	};
}

#endif