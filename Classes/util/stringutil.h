#pragma once

#include <string>

namespace BMS
{

	std::string trim(const std::string& input);
	std::string readLine(FILE* file);
	const int base36ToInt(const char* base36);
	const int base16ToInt(const char* base16);
	const int base10ToInt(const char* base10, int count = 3);
}