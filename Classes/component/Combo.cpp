#include "Combo.h"
#include "event/ComboEvent.h"
#include "scene/GameScene.h"

USING_NS_CC;

namespace BMS
{
	namespace
	{
		const char* COMBO_FONT_PATH = "fonts/combofont.fnt";
	}

	Combo* Combo::create()
	{
		auto ret = new Combo;
		if (ret && ret->init())
		{
			ret->autorelease();
			return ret;
		}

		return NULL;
	}

	bool Combo::init()
	{
		if (!Node::init())
			return false;

		mCombo = 0;

		mLabel = LabelBMFont::create();
		mLabel->setFntFile(COMBO_FONT_PATH);
		addChild(mLabel);

		Director::getInstance()->getEventDispatcher()->addCustomEventListener(Event::ComboEvent::CHANGED, std::bind(&Combo::onCombo, this, std::placeholders::_1));
		return true;
	}


	void Combo::onCombo(EventCustom* e)
	{
		Event::ComboEvent* comboEvent = static_cast<Event::ComboEvent*>(e->getUserData());
		mCombo = comboEvent->combo();

		// TODO: animation
		if(mCombo)
		{
			char strCombo[0xff];
			snprintf(strCombo, sizeof(strCombo), "%d", mCombo);
			mLabel->setString(strCombo);
			mLabel->setVisible(true);
		}
		else
		{
			mLabel->setVisible(false);
		}

		mLabel->setScale(2);
		mLabel->setOpacity(0xff);
		mLabel->stopAllActions();
		mLabel->runAction(CCSequence::createWithTwoActions(
			CCEaseOut::create(CCScaleTo::create(0.1f, 1, 1), 3),
			CCFadeOut::create(3)
		));
	}
}