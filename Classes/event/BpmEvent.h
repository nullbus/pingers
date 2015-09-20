#pragma once
#include "util/timeutil.h"

namespace BMS
{
namespace Event
{
	class BpmEvent
	{
	public:
		static const char* CHANGED;

	public:
		BpmEvent(float bpm, float meterPosition, float drawPosition) : mBpm(bpm), mTime(currentTime()), mMeterPosition(meterPosition), mDrawPosition(drawPosition) {}
		float bpm() const
		{
			return mBpm;
		}

		float timestamp() const
		{
			return mTime;
		}

		float meterPosition() const
		{
			return mMeterPosition;
		}

		float drawPosition() const
		{
			return mDrawPosition;
		}

	protected:
		float mBpm;
		float mTime;
		float mMeterPosition;
		float mDrawPosition;
	};
}
}