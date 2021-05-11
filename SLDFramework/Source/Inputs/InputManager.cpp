#include "InputManager.h"

#include "InputSetting.h"

uint32_t SLD::InputManager::CreateHashId(const std::string& str, InputEvent ie)
{
	// This function is from https://en.cppreference.com/w/cpp/utility/hash

	const uint32_t hashName = uint32_t(std::hash<std::string>{}(str));
	const uint32_t hashIE = uint32_t(std::hash<InputEvent>{}(ie));
	return hashName ^ (hashIE << 1);
}

//SLD::InputManager::InputManager(const CurrentWindow& handle)
//	: m_EventBuses()
//	, m_SubSystemInput(SFMLInputs{ handle })
//	, m_EventCntThisFrame()
//{
//}

SLD::InputManager::InputManager(LLInputs&& subSystem)
	: m_EventBuses()
	, m_SubSystemInput(std::move(subSystem))
	, m_EventCntThisFrame()
{
}

bool SLD::InputManager::TranslateWindowsMessages()
{
	const auto readOut = std::visit(LLInputRead{}, m_SubSystemInput);

	if (!readOut.isExit)
	{
		m_EventCntThisFrame = readOut.eventCnt;
		m_EventBuses = readOut.buses;
	}

	return readOut.isExit;
#pragma region InputSettingRegion

	//const auto& actionKeyMappings{ inputSettingRef.GetAllActionMappings() };
	//const auto& axisKeyMappings{ inputSettingRef.GetAllAxisMappings() };
	//
	//while (m_SFMLWindowHandle.pollEvent(ev))
	//{
	//	switch (ev.type)
	//	{
	//	case sf::Event::KeyPressed:

	//		const Key key{ InputDevice::D_Keyboard,uint16_t(ev.key.code) };
	//		const InputEvent inEv{ InputEvent::IE_Pressed };
	//		
	//		for (const auto& mapping : actionKeyMappings)
	//		{
	//			const auto& actionMap{ mapping.second };
	//			const auto& keyPool{ actionMap.keyPool };
	//			const auto& commands{ actionMap.commands };

	//			// check if key is exist
	//			if(keyPool.find(key) != keyPool.end())
	//			{
	//				// HOT ZONE
	//				for (const auto& cm : commands)
	//				{
	//					if(cm.iEvent == inEv)
	//					{
	//						cm.callback->Invoke();
	//					}
	//				}
	//			}
	//		}

	//		break;
	//	}
	//}

#pragma endregion 
}

SLD::EventQueueHandle SLD::InputManager::GetEventQueueHandle() const noexcept
{
	return { m_EventBuses,m_EventCntThisFrame };
}

//std::optional<const sf::Event*> SLD::InputManager::QuerySingleEvent(sf::Event::EventType evType) const noexcept
//{
//	for (uint8_t i = 0; i < m_EventCntThisFrame; ++i)
//	{
//		if (m_EventStack[i].type == evType)
//			return &m_EventStack[i];
//	}
//
//	return std::nullopt;
//}

//std::optional<std::vector<const sf::Event*>> SLD::InputManager::QueryMultipleEvent(
//	std::initializer_list<sf::Event::EventType> evTypes) const
//{
//	if (m_EventCntThisFrame)
//		return std::nullopt;
//
//	std::vector<const sf::Event*> outEvStack{};
//
//	for (uint8_t i = 0; i < m_EventCntThisFrame; ++i)
//	{
//		for (const auto& evType : evTypes)
//		{
//			const auto& temp{ m_EventStack[i] };
//			if (temp.type == evType)
//				outEvStack.emplace_back(&temp);
//		}
//	}
//
//	return { outEvStack };
//}

//SLD::EventQueueHandle SLD::InputManager::GetEventQueueHandle() const noexcept
//{
//	return EventQueueHandle{ m_EventStack,m_EventCntThisFrame };
//}
//
//SLD::InputManager::InputManager(InputManager&& other) noexcept
//	: m_EventStack(other.m_EventStack)
//	, m_SFMLWindowHandle(other.m_SFMLWindowHandle.getSystemHandle())
//	, m_EventCntThisFrame(other.m_EventCntThisFrame)
//{
//	other.m_SFMLWindowHandle.close();
//}
//
//SLD::InputManager& SLD::InputManager::operator=(InputManager&& other) noexcept
//{
//	if (this != &other)
//	{
//		m_EventStack = std::move(other.m_EventStack);
//		m_SFMLWindowHandle.create(other.m_SFMLWindowHandle.getSystemHandle());
//		other.m_SFMLWindowHandle.close();
//		m_EventCntThisFrame = other.m_EventCntThisFrame;
//		other.m_EventCntThisFrame = 0;
//	}
//
//	return *this;
//}