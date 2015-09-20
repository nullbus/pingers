/*
 *	Note
 *	노트 하나
 */

#ifndef __NOTE_H__
#define __NOTE_H__

#include "enums.h"
#include <string>

namespace BMS
{
	class BmsChannel;

	class BmsNote
	{
	friend class BmsChannel;

	public:

		BmsNote(float meterPosition, const std::string& value);
		bool operator < (const BmsNote& other) const;

		float meterPosition() const;
		float drawPosition() const;
		float floatValue() const;
		const std::string& value() const;

	protected:
		float mMeterPosition;		// 정수 부분: 마디, 소수 부분: 마디 내 오프셋
		float mDrawPosition;		// 노트가 그려질 위치 (기준 bpm으로 한 마디가 1)
		std::string mValue;
	};
}

#endif