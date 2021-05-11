#include "WindowCommandStack.h"

void SLD::WindowCommandStack::Push(sf::Event::EventType triggerEvent, const WindowEventType& eventPtr)
{
	m_CommandMaps[triggerEvent].emplace_back(eventPtr);
}

void SLD::WindowCommandStack::ShouldExecute(sf::Event::EventType condition, const sf::Event& eventValue) const noexcept
{
	auto fIt = m_CommandMaps.find(condition);
	if(fIt != m_CommandMaps.end())
	{
		const auto& commands{ fIt->second };
		for (const auto& cm : commands)
		{
			cm.lock()->Invoke(eventValue);
		}
	}
}
