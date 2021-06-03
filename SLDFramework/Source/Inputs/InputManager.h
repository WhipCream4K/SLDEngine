

#ifndef SLDFRAMEWORK_INPUTMANAGER_H
#define SLDFRAMEWORK_INPUTMANAGER_H

#include "../Core/Base.h"
#include "InputParams.h"
#include "LLInputs.h"

// SFML external dependencies
//#include <SFML/Window.hpp>

#include <optional>
#include <variant>

namespace SLD
{
	using namespace InputParams;
	
	struct EventQueueHandle
	{
		EventQueueHandle(const std::array<MessageBus, MinimumEventCnt>& ev,const uint8_t& evCnt)
			: events(ev)
			, eventCntThisFrame(evCnt)
		{
		}
		
		const std::array<MessageBus, MinimumEventCnt>& events;
		const uint8_t& eventCntThisFrame;
	};

	// SFML interface for this one
	// NOTE: I couldn't explicitly type all the key codes just for the sake of originality
	class InputManager final
	{
	public:

		using CurrentWindow = std::any;

		static uint32_t CreateHashId(const std::string& str, InputEvent ie);

		//InputManager(const CurrentWindow& handle);
		InputManager(LLInputs&& subSystem);

		/// <summary>
		/// From window API translate all the messages came through from the user
		/// and execute commands registered within current input setting
		/// </summary>
		/// <returns>return true if window exits</returns>
		bool TranslateWindowsMessages();

		[[nodiscard]] std::optional<const sf::Event*> QuerySingleEvent(
			sf::Event::EventType evType) const noexcept;

		[[nodiscard]] std::optional<std::vector<const sf::Event*>> QueryMultipleEvent(
			std::initializer_list<sf::Event::EventType> evTypes) const;

		[[nodiscard]] EventQueueHandle GetEventQueueHandle() const noexcept;

		template<typename ControllerType>
		std::optional<ControllerType> GetLowLevelController();
		
		InputManager(const InputManager&) = delete;
		InputManager& operator=(const InputManager&) = delete;

		InputManager(InputManager&& other) noexcept;
		InputManager& operator=(InputManager&& other) noexcept;

		~InputManager() = default;

	private:
		
		std::array<MessageBus, MinimumEventCnt> m_EventBuses;
		LLInputs m_SubSystemInput;
		uint8_t m_EventCntThisFrame;
	};

	template <typename ControllerType>
	std::optional<ControllerType> InputManager::GetLowLevelController()
	{
		if (auto ll = std::get_if<ControllerType>(&m_SubSystemInput))
			return ll;

		return std::nullopt;
	}
}

#endif