
#ifndef SLDFRAMEWORK_COMMANDQUEUE_H
#define SLDFRAMEWORK_COMMANDQUEUE_H

#include <unordered_map>
#include "../Core/VirMemDelegate.h"

#include <SFML/Window/Event.hpp>

namespace SLD
{
	// This class is couple with InputManager
	class WindowCommandStack
	{
	public:

		using WindowEventType = WeakPtr<VirMemDelegate<void(const sf::Event&)>>;

		void Push(sf::Event::EventType triggerEvent, const WindowEventType& eventPtr);

		// run time code
		void ShouldExecute(sf::Event::EventType condition, const sf::Event& eventValue) const noexcept;
		
	private:
		
		std::unordered_map<sf::Event::EventType, std::vector<WindowEventType>> m_CommandMaps;
	};
}

#endif
