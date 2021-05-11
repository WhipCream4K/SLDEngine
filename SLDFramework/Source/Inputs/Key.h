#ifndef SLDFRAMEWORK_KEY_H
#define SLDFRAMEWORK_KEY_H

#include "../Core/Base.h"

namespace SLD
{
	enum class InputDevice
	{
		D_Keyboard,
		D_Mouse,
		D_Gamepad
	};

	struct Key
	{
		using KeyType = uint16_t;
		
		Key(InputDevice device, KeyType code)
			: inputDevice(device)
			, keyCode(code)
		{
		}

		bool operator!=(const Key& other) const
		{
			return inputDevice != other.inputDevice &&
				keyCode != other.keyCode;
		}

		bool operator==(const Key& other) const
		{
			return inputDevice == other.inputDevice &&
				keyCode == other.keyCode;
		}

		bool operator<(const Key& other) const
		{
			return inputDevice < other.inputDevice ||
				keyCode < other.keyCode;
		}

		InputDevice inputDevice;
		KeyType keyCode;
	};
	
	struct KeyHasher
	{
		size_t operator()(const Key& key) const
		{
			return std::hash<decltype(key.keyCode)>{}(key.keyCode) ^
				(std::hash<decltype(key.inputDevice)>{}(key.inputDevice) << 1);
		}

		size_t operator()(uint16_t exterKey,InputDevice device) const
		{
			return std::hash<uint16_t>{}(exterKey) ^
				(std::hash<InputDevice>{}(device) << 1);
		}
	};
}

#endif
