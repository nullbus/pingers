#pragma once

namespace BMS
{
namespace Event
{
	
	class ComboEvent
	{
	public:
		static const char* CHANGED;

	public:
		ComboEvent(int combo, int score) : mCombo(combo), mScore(score) {}
		int combo()
		{
			return mCombo;
		}
		int score()
		{
			return mScore;
		}

	protected:
		int mCombo;
		int mScore;
	};
}
}