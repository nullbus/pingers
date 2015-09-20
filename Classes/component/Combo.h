#pragma once
#include <cocos2d.h>

namespace BMS
{
	class Combo : public cocos2d::Node
	{
	public:
		static Combo* create();
		virtual bool init() override;

	protected:
		void onCombo(cocos2d::EventCustom* obj);

	protected:
		int mCombo;
		cocos2d::LabelBMFont* mLabel;
	};

}