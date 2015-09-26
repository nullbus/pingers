#include "timeutil.h"
#include <chrono>

namespace BMS
{
	float currentTime()
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
		return ms.time_since_epoch().count() / 1000.0;
	}
}