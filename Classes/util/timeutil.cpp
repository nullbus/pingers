#include "timeutil.h"
#include <Windows.h>

namespace BMS
{
	float currentTime()
	{
		LARGE_INTEGER counter, frequency;
		if(FALSE == QueryPerformanceCounter(&counter))
		{
			return GetTickCount64() / 1000.0;
		}
		else
		{
			QueryPerformanceFrequency(&frequency);
			return counter.QuadPart / float(frequency.QuadPart);
		}
	}
}