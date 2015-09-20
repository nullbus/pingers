#pragma once
#include <map>
#include <string>
#include "game/Song.h"
#include "BmsChannel.h"

namespace BMS
{
	class BmsParser;

	class BmsDocument
	{
	friend class BmsParser;

	public:
		~BmsDocument()
		{
			for(auto it = mChannels.begin(); it != mChannels.end(); it++)
			{
				delete it->second;
			}
		}

		const SongInfo& header() const
		{
			return mHeader;
		}

		const std::map<std::string, BmsChannel*>& channel() const
		{
			return mChannels;
		}

		const std::map<std::string, std::string>& bga() const
		{
			return mBGAs;
		}

		const std::map<std::string, std::string>& wave() const
		{
			return mWaves;
		}

		const std::map<std::string, float>& bpm() const
		{
			return mBpms;
		}

	protected:
		SongInfo mHeader;
		std::map<std::string, BmsChannel*> mChannels;
		std::map<std::string, std::string> mBGAs;
		std::map<std::string, std::string> mWaves;
		std::map<std::string, float> mBpms;
	};
}