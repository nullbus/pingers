#include "BmsParser.h"
#include "util/stringutil.h"
#include "util/containerutil.h"
#include "game/Song.h"
#include "BmsDocument.h"
#include <algorithm>
#include <string.h>

#define HEADERCMP(string, key) (0 == strncasecmp(string.c_str(), #key, strlen(#key)))

namespace BMS
{
	namespace ChannelType
	{
#define TYPE_DEFINE(name, number) const char* name = #number;
#include "ChannelTypes.inc"
#undef	TYPE_DEFINE
	}
	bool BmsParser::parseHeader(const char* path, SongInfo& songinfo)
	{

		FILE* f = NULL;
		songinfo.mPath = path;

		try {
			f = fopen(path, "r");

			if(!f) {
				throw 1;
			}
			//TODO: 읽기 다 만들기
			while(!feof(f))
			{
				std::string line = trim( readLine(f) );
				if(line.length() == 0 || line[0] != '#')
					continue;
				
				if(HEADERCMP(line, #TITLE))
				{
					songinfo.mTitle = parseArgument(line);
				}
				else if(HEADERCMP(line, #GENRE))
				{
					songinfo.mGenre = parseArgument(line);
				}
				else if(HEADERCMP(line, #ARTIST))
				{
					songinfo.mArtist = parseArgument(line);
				}
				else if(HEADERCMP(line, #LNTYPE))
				{
					songinfo.mLongnoteType = parseInt(line);
				}
				else if(HEADERCMP(line, #STAGEFILE))
				{
					songinfo.mStageFile = parseArgument(line);
				}
				else if(HEADERCMP(line, #BPM))
				{
					songinfo.mBpm = parseFloat(line);
					if(songinfo.mBpm <= 0)
						throw 2;
				}
				else if(HEADERCMP(line, #PLAYER))
				{
					int playType = PLAYTYPE( parseInt(line) );
					switch(playType)
					{
					case BMS::SINGLE:
					case BMS::COPY:
						if(strstr(line.c_str(), ".bme"))
						{
							songinfo.mKey = KEY_7;
						}
						else
						{
							songinfo.mKey = KEY_5;
						}
						break;

					case BMS::DUET:
					case BMS::DOUBLE:
						if(strstr(line.c_str(), ".bme"))
						{
							songinfo.mKey = KEY_14;
						}
						else
						{
							songinfo.mKey = KEY_10;
						}
						break;

					default:
						throw 3;
					}
				}
				else if(HEADERCMP(line, #PLAYLEVEL))
				{
					songinfo.mLevel = std::min(99, std::max(0, parseInt(line)));
				}
				else if(HEADERCMP(line, #RANK))
				{
					songinfo.rank = std::min<RANK>(VHARD, std::max<RANK>(EASY, RANK(parseInt(line))));
				}
			}
		}
		catch(int)
		{
			if(f) fclose(f);
			return false;
		}

		fclose(f);
		return true;
	}

	bool BmsParser::compile(const char* path, BmsDocument& document)
	{
		if(false == parseHeader(path, document.mHeader))
			return false;

		clearMap(document.mBGAs);
		clearMap(document.mBpms);
		clearPointerMap(document.mChannels);
		clearMap(document.mWaves);

		FILE *f = fopen(document.mHeader.getPath().c_str(), "r");
		string errstr;
		if(!f)
		{
			return false;
		}

		while(!feof(f))
		{
			std::string line = trim( readLine(f) );
			if(line.length() < 6 || line.at(0) != '#')
				continue;

			//#WAV 처리
			if(HEADERCMP(line, #WAV))
			{
				std::string index = line.substr(4, 2);
				int offset = line.find(" ");
				document.mWaves[index] = trim( line.substr(offset + 1) );
			}
			//#BMP 처리
			else if(HEADERCMP(line, #BMP))
			{
				std::string index = line.substr(4, 2);
				int offset = line.find(" ");
				document.mBGAs[index] = trim( line.substr(offset + 1) );
			}
			//일반 노트 처리
			else if(line.length() >= 7 && line[0] == '#' && line[6] == ':')
			{
				int bar = atoi( line.substr(1, 3).c_str() );
				std::string channel = line.substr(4, 2);
				auto it = document.mChannels.find(channel);
				if(it == document.mChannels.end())
				{
					BmsChannel* newChannel;
					if(channel.find(ChannelType::BAR_LENGTH) != std::string::npos)
					{
						newChannel = new ScaleChannel;
					}
					else if(channel.find(ChannelType::BPM_CHANGE) != std::string::npos)
					{
						newChannel = new BpmChannel;
					}
					else
					{
						newChannel = new BmsChannel;
					}
					document.mChannels[channel] = newChannel;
				}

				int colonIndex = line.find(":");
				document.mChannels[channel]->parseBar(bar, line.c_str() + colonIndex + 1);
			}
		}

		fclose(f);

		//노트 정렬
		int maxLength = 0;
		for(auto it = document.mChannels.begin(); it != document.mChannels.end(); it++)
		{
			it->second->sortNotes();
			if(it->second->endPosition() > maxLength)
				maxLength = it->second->endPosition();
		}

		// 마디선을 그리기 위한 노트
		document.mChannels[ChannelType::METERLINE] = new MeterLineChannel(maxLength);

		// 노트의 위치를 계산
		auto itScale = document.mChannels.find(ChannelType::BAR_LENGTH);
		if(itScale != document.mChannels.end())
		{
			for(auto itChannel = document.mChannels.begin(); itChannel != document.mChannels.end(); itChannel++)
			{
				itChannel->second->applyScaleChannel(static_cast<ScaleChannel*>(itScale->second));
			}
		}

		return true;
	}

	std::string BmsParser::parseArgument( std::string& line )
	{
		unsigned index = line.find(' ');
		if(index != std::string::npos) {
			return trim( line.substr(index) );
		}

		return "";
	}

	float BmsParser::parseFloat(std::string& line)
	{
		std::string argument = parseArgument(line);
		return atof(argument.c_str());
	}

	int BmsParser::parseInt(std::string& line)
	{
		std::string argument = parseArgument(line);
		return atoi(argument.c_str());
	}


}

#undef HEADERCMP