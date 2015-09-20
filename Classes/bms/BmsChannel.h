#pragma once
#include <vector>
#include "enums.h"

namespace BMS
{
	class BmsNote;
	class BpmChannel;
	class ScaleChannel;

	class BmsChannel
	{
	public:
		BmsChannel();
		virtual ~BmsChannel();

		void clear();
		void sortNotes();
		virtual void parseBar(int bar, const std::string& line);
		float endPosition() const;
		const std::vector<BmsNote*>& notes() const;

		void applyScaleChannel(const ScaleChannel* scaleChannel);

	protected:
		std::vector<BmsNote*> mNotes;
	};

	class BpmChannel : public BmsChannel
	{
	public:
		virtual void parseBar(int bar, const std::string& line) override;
	};

	class ScaleChannel : public BmsChannel
	{
	public:
		virtual void parseBar(int bar, const std::string& line) override;
	};

	class MeterLineChannel : public BmsChannel
	{
	public:
		MeterLineChannel(int length);
		// do nothing
		virtual void parseBar(int bar, const std::string& line) override {}
	};
}

