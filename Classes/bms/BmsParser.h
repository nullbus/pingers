#pragma once
#include <string>

namespace BMS
{
	class SongInfo;
	class BmsDocument;

	namespace ChannelType
	{
#define TYPE_DEFINE(name, number) extern const char* name;
#include "ChannelTypes.inc"
#undef	TYPE_DEFINE
	}

	class BmsParser
	{
	public:
		bool compile(const char* path, BmsDocument& document);
		bool parseHeader(const char* path, SongInfo& songinfo);

	protected:
		std::string parseArgument(std::string& line);
		float parseFloat(std::string& line);
		int parseInt(std::string& line);
	};

}