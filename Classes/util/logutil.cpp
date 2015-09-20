#include "logutil.h"

namespace BMS
{
namespace Log
{
	static void log_valist(const char* format, va_list args)
	{
		char buffer[0x1000];
		vsprintf(buffer, format, args);
		OutputDebugStringA(buffer);
		OutputDebugStringA("\n");
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