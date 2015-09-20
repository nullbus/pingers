#pragma once

namespace BMS
{
	class SongInfo;

namespace Event
{
	class GameConfigEvent
	{
	public:
		static const char* RESET;

	public:
		GameConfigEvent(const BMS::SongInfo& songInfo, float speed, bool autoPlay, float startTime) :
			mSongInfo(songInfo),
			mSpeed(speed),
			mAutoPlay(autoPlay),
			mStartTime(startTime)
		{

		}

		const BMS::SongInfo& songInfo() const
		{
			return mSongInfo;
		}

		bool autoPlay() const 
		{
			return mAutoPlay;
		}

		float speed() const
		{
			return mSpeed;
		}

		float startTime() const
		{
			return mStartTime;
		}

	protected:
		const BMS::SongInfo& mSongInfo;
		bool mAutoPlay;
		float mSpeed;
		float mStartTime;
	};
}
}