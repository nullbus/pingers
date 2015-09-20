#pragma once

namespace BMS
{
	float currentTime();
	inline float calculateMeter(float bpm, float playTime)
	{
		return bpm * playTime / 4 / 60;
	}
}