#pragma once
#include "definitions.h"
#include "enums.h"

namespace BMS
{
namespace Event
{
	class JudgeEvent
	{
	public:
		static const char* JUDGE;

	public:
		JudgeEvent(BMS::JUDGE judge, Key::MappedKey key) : mJudge(judge), mKey(key)
		{

		}
		BMS::JUDGE judge()
		{
			return mJudge;
		}
		Key::MappedKey key()
		{
			return mKey;
		}

	protected:
		BMS::JUDGE mJudge;
		Key::MappedKey mKey;
	};
}
}