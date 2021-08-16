#include "InputSettings.h"
//#include <Windows.h>

const std::vector<SLD::ActionKey>& SLD::InputSettings::GetActionKeyByName(const std::string& groupName) const
{
	return m_ActionKeyMappings.at(groupName).keys;
}

const std::vector<SLD::AxisKey>& SLD::InputSettings::GetAxisKeyByName(const std::string& groupName) const
{
	return m_AxisKeyMappings.at(groupName).keys;
}

void SLD::InputSettings::AddActionMapping(const std::string& groupName, std::initializer_list<ActionKey> keys)
{
	//ActionMap::KeyPoolType keyPool{};

	for (const auto& val : keys)
	{
		if (const auto fintItr{ m_MainKeyPool.find(val.key) };
			fintItr == m_MainKeyPool.end())
		{
			m_MainKeyPool[val.key] = InputState{};
		}
	}

	m_ActionKeyMappings.try_emplace(groupName, ActionMap{ {keys} });
}

void SLD::InputSettings::AddAxisMapping(const std::string& groupName, std::initializer_list<AxisKey> keys)
{
	//AxisMap::KeyPoolType keyPool{};

	for (const auto& val : keys)
	{
		if (const auto fintItr{ m_MainKeyPool.find(val.key) };
			fintItr == m_MainKeyPool.end())
		{
			m_MainKeyPool[val.key] = InputState{};
		}
	}

	m_AxisKeyMappings.try_emplace(groupName, AxisMap{ {keys} });
}

void SLD::InputSettings::AddActionKeyToMapping(
	const std::string& actionMappingName,
	const ActionKey& key,
	bool rebuildKeyPool)
{
	auto& actionMap{ m_ActionKeyMappings.at(actionMappingName) };
	auto& actionKeys{ actionMap.keys };
	auto& keyPool{ actionMap.keyPool };

	// check if duplicate
	const auto fIt = keyPool.find(key.key);
	if (fIt == keyPool.end())
	{
		actionKeys.emplace_back(key);
		if (rebuildKeyPool) // for this one just add it to the Keypool
			keyPool.insert(key.key);

	}
}

void SLD::InputSettings::AddAxisKeyToMapping(const std::string& axisMappingName, const AxisKey& key, bool rebuildKeyPool)
{
	auto& axisMap{ m_AxisKeyMappings.at(axisMappingName) };
	auto& axisKeys{ axisMap.keys };
	auto& keyPool{ axisMap.keyPool };

	// check if duplicate
	const auto& fIt = keyPool.find(key.key);
	if (fIt == keyPool.end())
	{
		axisKeys.emplace_back(key);
		if (rebuildKeyPool)
			keyPool.try_emplace(key.key, key.scale);
	}
}

void SLD::InputSettings::AddActionKeysToMapping(
	const std::string& actionMappingName,
	const std::vector<ActionKey>& keyVec,
	bool rebuildKeyPool)
{
	actionMappingName;
	keyVec;
	rebuildKeyPool;
}

void SLD::InputSettings::AddAxisKeysToMapping(
	const std::string& axisMappingName,
	const std::vector<AxisKey>& keyVec,
	bool rebuildKeyPool)
{
	axisMappingName;
	keyVec;
	rebuildKeyPool;
}

void SLD::InputSettings::RemoveActionKeyFromMapping(const std::string& groupName, const ActionKey& key,
	bool rebuildKeyPool)
{
	groupName;
	key;
	rebuildKeyPool;
}

void SLD::InputSettings::RemoveAxisKeyFromMapping(const std::string& groupName, const AxisKey& key, bool rebuildKeyPool)
{
	groupName;
	key;
	rebuildKeyPool;
}

float SLD::InputSettings::GetInputAxis(const std::string& groupName, uint8_t* playerIndex)
{
	AxisMap& axis{ m_AxisKeyMappings.at(groupName) };

	for (const auto& key : axis.keys)
	{
		InputState& state{ m_MainKeyPool.at(key.key) };

		if (state.keyEvent == InputEvent::IE_Down ||
			state.keyEvent == InputEvent::IE_Pressed)
		{
			if (playerIndex)
				*playerIndex = state.userIndex;
			
			return key.scale * state.axisValue;
		}
	}

	return {};
}

SLD::InputEvent SLD::InputSettings::GetInputState(const std::string& groupName, uint8_t* playerIndex)
{
	ActionMap& action{ m_ActionKeyMappings.at(groupName) };

	for (const auto& key : action.keys)
	{
		InputState& state{ m_MainKeyPool.at(key.key) };
		if (int(state.keyEvent) < int(InputEvent::IE_None))
		{
			if (playerIndex)
				*playerIndex = state.userIndex;
			
			return state.keyEvent;
		}
	}

	return InputEvent::IE_None;
}

void SLD::InputSettings::ParseMessage(const InputParams::MessageBus& message) const
{
	using namespace InputParams;

	switch (message.eventId)
	{
	case EventType::KeyReleased:
	{
		const Key inKey{ InputDevice::D_Keyboard,uint16_t(message.data.keyboard.key) };
		const InputEvent ie{ InputEvent::IE_Released };

		for (const auto& actionMap : m_ActionKeyMappings)
		{
			const auto& keyPool{ actionMap.second.keyPool };
			const auto& commands{ actionMap.second.commandTable };

			if (keyPool.find(inKey) != keyPool.end())
			{
				for (const auto& cm : commands)
				{
					if (cm.iEvent == ie)
						cm.callback->Invoke();
				}
			}
		}

		break;
	}
	case EventType::KeyPressed:
	{
		const Key inKey{ InputDevice::D_Keyboard,uint16_t(message.data.keyboard.key) };
		//const InputEvent ie{ InputEvent::IE_Pressed };
		const float axisValue{ 1.0f };
		//OutputDebugString(L"1\n");
		for (const auto& axisMap : m_AxisKeyMappings)
		{
			const auto& keyPool{ axisMap.second.keyPool };
			const auto& commands{ axisMap.second.commandTable };

			const auto fIt = keyPool.find(inKey);
			if (fIt != keyPool.end())
			{
				const float totalAxisValue{ fIt->second * axisValue };
				for (const auto& cm : commands)
				{
					cm.callback->Invoke(totalAxisValue);
				}
			}
		}

		break;
	}
	case XInputState:
	{
		const Key inKey{ InputDevice::D_Gamepad,uint16_t(message.data.joyStick.joyStickButton) };

		// TODO: Fix this limit controller behaviour
		const InputEvent ie{ InputEvent::IE_Released };


		for (const auto& actionMap : m_ActionKeyMappings)
		{
			const auto& keyPool{ actionMap.second.keyPool };
			const auto& commands{ actionMap.second.commandTable };

			if (keyPool.find(inKey) != keyPool.end())
			{
				for (const auto& cm : commands)
				{
					if (cm.iEvent == ie)
						cm.callback->Invoke();
				}
			}
		}

		float axisValue{};
		switch (GamePadKey(message.data.joyStick.joyStickButton))
		{
		case GamePadKey::GPK_Left_AxisX: axisValue = message.data.joyStick.leftThumbX; break;
		case GamePadKey::GPK_Left_AxisY: axisValue = message.data.joyStick.leftThumbY; break;
		case GamePadKey::GPK_Left_Shoulder:	axisValue = message.data.joyStick.leftTrigger; break;
		case GamePadKey::GPK_Right_Shoulder:	axisValue = message.data.joyStick.rightTrigger; break;
		default: axisValue = 1.0f;
		}

		for (const auto& axisMap : m_AxisKeyMappings)
		{
			const auto& keyPool{ axisMap.second.keyPool };
			const auto& commands{ axisMap.second.commandTable };

			const auto fIt = keyPool.find(inKey);
			if (fIt != keyPool.end())
			{
				const float totalAxisValue{ fIt->second * axisValue };
				for (const auto& cm : commands)
				{
					cm.callback->Invoke(totalAxisValue);
				}
			}
		}

	}
	break;
	default: break;
	}

}

void SLD::InputSettings::RemoveCommands(WeakPtr<GameObject> reference)
{
	// Delete from both command table

	for (auto& keyMap : m_ActionKeyMappings)
	{
		auto& commandTable{ keyMap.second.commandTable };

		commandTable.erase(std::remove_if(commandTable.begin(), commandTable.end(), [&reference](const ActionCommand& item)
			{
				return item.referencePointer.lock() == reference.lock();
			}), commandTable.end());
	}

	for (auto& keyMap : m_AxisKeyMappings)
	{
		auto& commandTable{ keyMap.second.commandTable };
		commandTable.erase(std::remove_if(commandTable.begin(), commandTable.end(), [&reference](const AxisCommand& item)
			{
				return item.referencePointer.lock() == reference.lock();
			}));
	}
}

SLD::InputSettings::~InputSettings()
{
	for (auto& keyMap : m_ActionKeyMappings)
	{
		auto& commandTable{ keyMap.second.commandTable };
		commandTable.clear();
	}

	for (auto& keyMap : m_AxisKeyMappings)
	{
		auto& commandTable{ keyMap.second.commandTable };
		commandTable.clear();
	}
}