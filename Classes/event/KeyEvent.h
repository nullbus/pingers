#pragma once
#include "enums.h"

namespace BMS
{
namespace Event
{
	class KeyEvent
	{
	public:
		static const char* DOWN;
		static const char* UP;

	public:
		KeyEvent(Key::MappedKey key, Key::KeyState state, float pos)
		{
			mKey = key;
			mState = state;
			mPos = pos;
		}

		Key::MappedKey keyCode()
		{
			return mKey;
		}

		Key::KeyState state()
		{
			return mState;
		}

		float pos()
		{
			return mPos;
		}

	protected:
		Key::MappedKey mKey;
		Key::KeyState mState;
		float mPos;
	};
}
}