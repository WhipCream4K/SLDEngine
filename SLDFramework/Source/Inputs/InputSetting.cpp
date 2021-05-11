#include "InputSetting.h"

const std::vector<SLD::ActionKey>& SLD::InputSetting::GetActionKeyByName(const std::string& groupName) const
{
	return m_ActionKeyMappings.at(groupName).keys;
}

const std::vector<SLD::AxisKey>& SLD::InputSetting::GetAxisKeyByName(const std::string& groupName) const
{
	return m_AxisKeyMappings.at(groupName).keys;
}

void SLD::InputSetting::AddActionMapping(const std::string& groupName, std::initializer_list<ActionKey> keys)
{
	ActionMap::KeyPoolType keyPool{};

	for (const auto& val : keys)
	{
		auto fIt = keyPool.find(val.key);
		if (fIt == keyPool.end())
			keyPool.insert(val.key);
	}

	ActionMap actionMap{ {keys},keyPool };
	m_ActionKeyMappings.try_emplace(groupName, actionMap);
}

void SLD::InputSetting::AddAxisMapping(const std::string& groupName, std::initializer_list<AxisKey> keys)
{
	AxisMap::KeyPoolType keyPool{};

	for (const auto& val : keys)
	{
		keyPool.try_emplace(val.key, val.scale);
	}

	AxisMap axisMap{ {keys},keyPool };
	m_AxisKeyMappings.try_emplace(groupName, axisMap);
}

void SLD::InputSetting::AddActionKeyToMapping(
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
		{
			keyPool.insert(key.key);
		}
	}
}

void SLD::InputSetting::AddAxisKeyToMapping(const std::string& axisMappingName, const AxisKey& key, bool rebuildKeyPool)
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

void SLD::InputSetting::AddActionKeysToMapping(
	const std::string& actionMappingName,
	const std::vector<ActionKey>& keyVec,
	bool rebuildKeyPool)
{
	actionMappingName;
	keyVec;
	rebuildKeyPool;
}

void SLD::InputSetting::AddAxisKeysToMapping(
	const std::string& axisMappingName,
	const std::vector<AxisKey>& keyVec,
	bool rebuildKeyPool)
{
	axisMappingName;
	keyVec;
	rebuildKeyPool;
}

void SLD::InputSetting::ParseMessage(const MessageBus& message) const
{
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
		const InputEvent ie{ InputEvent::IE_Pressed };
		const float axisValue{ 1.0f };

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
					cm->Invoke(totalAxisValue);
				}
			}
		}

		break;
	}

	default: break;
	}

}
