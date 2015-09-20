#include "BmsNote.h"

namespace BMS
{
	BmsNote::BmsNote(float meterPosition, const std::string& value):
		mMeterPosition(meterPosition),
		mValue(value),
		mDrawPosition(meterPosition)
	{
	}

	float BmsNote::meterPosition() const
	{
		return mMeterPosition;
	}

	float BmsNote::drawPosition() const
	{
		return mDrawPosition;
	}

	float BmsNote::floatValue() const
	{
		return atof(mValue.c_str());
	}

	const std::string& BmsNote::value() const
	{
		return mValue;
	}

	bool BmsNote::operator<(const BmsNote& other) const
	{
		return (mMeterPosition < other.mMeterPosition);
	}

}