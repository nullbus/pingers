/*
 *	Song
 *	노래 클래스
 */

#ifndef __SONG_H__
#define __SONG_H__

#include <string>
#include "definitions.h"
#include <boost/filesystem.hpp>

namespace BMS
{
	class BmsParser;

	class SongInfo  
	{
	friend class BmsParser;
	public:
		SongInfo();

		boost::filesystem::path getParentPath() const
		{
			return mPath.parent_path();
		}

		const std::string& getTitle() const { return mTitle; }
		const std::string& getGenre() const { return mGenre; }
		const std::string& getArtist() const { return mArtist; }
		std::string getPath() const { return mPath.string(); }
		const std::string& getStageFile() const { return mStageFile; }
		float getBpm() const { return mBpm; }
		int getLevel() const { return mLevel; }
		NUMKEY getNumKey() const { return mKey; }

	private:
		void init();

		boost::filesystem::path mPath;
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
