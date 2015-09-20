#include "Song.h"
#include "util/stringutil.h"


namespace BMS
{
	SongInfo::SongInfo()
	{
		init();
	}

	void SongInfo::init()
	{
		mBpm = 0;
		mLevel = 3;
		mKey = KEY_5;
		rank = EASY;
		volume = 100;
	}
}