#pragma once
#include <stdio.h>
#include <vadefs.h>
#include <Windows.h>

namespace BMS
{
	namespace Log
	{
		void i(const char* format, ...);
		void d(const char* format, ...);
	}
}