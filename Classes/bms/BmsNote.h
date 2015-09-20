/*
 *	Note
 *	��Ʈ �ϳ�
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
		float mMeterPosition;		// ���� �κ�: ����, �Ҽ� �κ�: ���� �� ������
		float mDrawPosition;		// ��Ʈ�� �׷��� ��ġ (���� bpm���� �� ���� 1)
		std::string mValue;
	};
}

#endif