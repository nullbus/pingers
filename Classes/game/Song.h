/*
 *	Song
 *	노래 클래스
 */

#ifndef __SONG_H__
#define __SONG_H__

#include <string>
#include "definitions.h"

namespace BMS
{
	class BmsParser;

	class SongInfo  
	{
	friend class BmsParser;
	public:
		SongInfo();

		const std::string getParentPath() const
		{
			return mPath.substr(0, mPath.find_last_of('/')+1);
		}

		const std::string& getTitle() const { return mTitle; }
		const std::string& getGenre() const { return mGenre; }
		const std::string& getArtist() const { return mArtist; }
		const std::string& getPath() const { return mPath; }
		const std::string& getStageFile() const { return mStageFile; }
		float getBpm() const { return mBpm; }
		int getLevel() const { return mLevel; }
		NUMKEY getNumKey() const { return mKey; }

	private:
		void init();

		std::string mPath;
		std::string mTitle;
		std::string mGenre;
		std::string mArtist;
		std::string mStageFile;
		NUMKEY mKey;
		float mBpm;
		PLAYTYPE mPlayType;
		int mLevel;
		int mLongnoteType;	// #LNTYPE X

		RANK rank;
		int volume;
	};
}

#endif
