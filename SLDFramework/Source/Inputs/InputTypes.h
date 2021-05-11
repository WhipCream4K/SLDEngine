#ifndef SLDFRAMEWORK_INPUTTYPES_H
#define SLDFRAMEWORK_INPUTTYPES_H

#include "Key.h"

namespace SLD
{
	struct ActionKey
	{
		ActionKey(const Key& defKey)
			:key(defKey)
		{
		}

		bool operator!=(const ActionKey& other) const
		{
			return key.keyCode != other.key.keyCode;
		}

		bool operator==(const ActionKey& other) const
		{
			return key.keyCode == other.key.keyCode;
		}
		
		Key key;
	};

	struct AxisKey
	{
		AxisKey(const Key& defKey, float scaleValue)
			:key(defKey)
			, scale(scaleValue)
		{
		}
		
		Key key;
		float scale;
	};
}

#endif

