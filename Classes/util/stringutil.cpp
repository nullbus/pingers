#include "stringutil.h"
#include <algorithm>
#include <functional>
#include <locale>
#include <cctype>
#include <stdio.h>
#include <assert.h>

namespace
{
	std::string& ltrim(std::string& str)
	{
		auto it2 = std::find_if(str.begin(), str.end(), [](char ch) { return !std::isspace<char>(ch, std::locale::classic()); });
		str.erase(str.begin(), it2);
		return str;
	}

	std::string& rtrim(std::string& str)
	{
		auto it1 = std::find_if(str.rbegin(), str.rend(), [](char ch) { return !std::isspace<char>(ch, std::locale::classic()); });
		str.erase(it1.base(), str.end());
		return str;
	}
}
namespace BMS
{
	std::string trim(const std::string& str)
	{
		auto str_copy = str;
		return ltrim(rtrim(str_copy));
	}

	std::string readLine(FILE* file)
	{
		assert(file);

		if(feof(file))
			return "";

		char buf[0x1000];
		char* ret = fgets(buf, sizeof(buf), file);
		
		if(NULL == ret)
			return "";

		return buf;
	}

	const int base36ToInt(const char* base36)
	{
		if(NULL == base36 || strlen(base36) < 2)
		{
			assert(false && "not base36 integer!");
			return -1;
		}

		char ten = base36[0];
		char one = base36[1];
		return ((ten + (ten>='A'?10-'A':-'0'))*36) + one+(one>='A'?10-'A':-'0');
	}

	const int base16ToInt(const char* base16)
	{
		if(NULL == base16 || strlen(base16) < 2)
		{
			assert(false && "invalid base16 integer!");
			return -1;
		}

		char ten = base16[0];
		char one = base16[1];
		return ((ten + (ten>='A'?10-'A':-'0'))*16) + one+(one>='A'?10-'A':-'0');
	}

	const int base10ToInt(const char* base10, int count /* = 3 */)
	{
		if(NULL == base10 || strlen(base10) < count)
		{
			assert(false && "invalid base10 integer!");
			return -1;
		}

		int ret = 0;
		for(int i=0; i<count; i++)
		{
			ret *= 10;
			ret += base10[i] - '0';
		}

		return ret;
	}
}