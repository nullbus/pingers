#include "logutil.h"
#include <cstdarg>

#ifdef _MSC_VER
#include <Windows.h>
#endif

namespace BMS
{
namespace Log
{
	static void log_valist(const char* format, va_list args)
	{
		char buffer[0x1000];
		vsprintf(buffer, format, args);
#ifdef _MSC_VER
		OutputDebugStringA(buffer);
		OutputDebugStringA("\n");
#else
		printf(buffer);
#endif
	}

	void i(const char* format, ...)
	{
		va_list list;
		va_start(list, format);
		log_valist(format, list);
		va_end(list);
	}

	void d(const char* format, ...)
	{
#		ifdef _DEBUG
		va_list list;
		va_start(list, format);
		log_valist(format, list);
		va_end(list);
#		endif
	}
}
}