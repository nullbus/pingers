#include "BmsChannel.h"
#include <string>
#include <algorithm>
#include <assert.h>
#include "util/stringutil.h"
#include "util/containerutil.h"
#include "util/logutil.h"
#include "BmsNote.h"

namespace BMS
{

	BmsChannel::BmsChannel()
	{
	}

	BmsChannel::~BmsChannel()
	{
		clear();
	}

	void BmsChannel::clear()
	{
		clearPointerVector(mNotes);
	}

	static bool sortNotePointerFunc(const BmsNote* lhs, const BmsNote* rhs)
	{
		return *lhs < *rhs;
	}

	void BmsChannel::sortNotes()
	{
		std::sort(mNotes.begin(), mNotes.end(), sortNotePointerFunc);
	}

	void BmsChannel::parseBar(int bar, const std::string& line)
	{
		// TODO parse line
		assert(0 == line.length() % 2);
		int numNote = line.length() / 2;

		for(int i=0; i<numNote; i++)
		{
			if(line[i*2] == '0' && line[i*2+1] == '0')
				continue;

			std::string value = line.substr(i*2, 2);
			float pos = bar + (i / float(numNote));
			mNotes.push_back(new BmsNote(pos, value));
		}
	}

	float BmsChannel::endPosition() const
	{
		if(mNotes.empty())
			return 0;

		return mNotes[mNotes.size() - 1]->meterPosition();
	}

	const std::vector<BmsNote*>& BmsChannel::notes() const
	{
		return mNotes;
	}

	void BmsChannel::applyScaleChannel(const ScaleChannel* scaleChannel)
	{
		int beginNote = 0;
		auto notes = this->notes();
		auto itScaleNote = scaleChannel->notes().begin();
		while(itScaleNote != scaleChannel->notes().end())
		{
			float beginPosition = (*itScaleNote)->meterPosition();
			float scale = (*itScaleNote)->floatValue();
			Log::d("scale changes %.2f at %.3f", scale, beginPosition);

			// find scale begin position
			bool bFound = false;
			for(unsigned i = beginNote; i < notes.size(); i++)
			{
				if(notes[i]->meterPosition() >= beginPosition)
				{
					beginNote = i;
					bFound = true;
					break;
				}
			}

			if(!bFound)
			{
				// if then, there are nothing to do
				break;
			}

			// scale 1 meter
			int endNote = notes.size();
			for(unsigned i = beginNote; i < notes.size(); i++)
			{
				if(notes[i]->meterPosition() >= beginPosition+1)
				{
					endNote = i;
					break;
				}

				float posInMeter = notes[i]->meterPosition() - (*itScaleNote)->meterPosition();
				float pull = posInMeter * (1 - scale);
				notes[i]->mDrawPosition -= pull;
			}

			// apply latter effects of scale
			float pull = 1.0f - scale;
			for (unsigned i = endNote; i < notes.size(); i++)
			{
				notes[i]->mDrawPosition -= pull;
			}

			itScaleNote++;
		}
	}

	void BpmChannel::parseBar(int bar, const std::string& line)
	{
		assert(0 == line.length() % 2);
		int numNote = line.length() / 2;

		for(int i=0; i<numNote; i++)
		{
			std::string value = line.substr(i*2, 2);
			float pos = bar + (i / float(numNote));
			mNotes.push_back(new BmsNote(pos, value));
		}
	}

	// ScaleChannel functions
	void ScaleChannel::parseBar(int bar, const std::string& line)
	{
		// line is float value string
		mNotes.push_back(new BmsNote(bar, line));
	}


	// MeterLineChannel functions
	MeterLineChannel::MeterLineChannel(int length)
	{
		for(int i=0; i<length+1; i++)
			mNotes.push_back(new BmsNote(i, ""));
	}

}