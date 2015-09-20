#pragma once
#include <cocos2d.h>

USING_NS_CC;

namespace BMS
{
	class Jukebox;

	class JukeboxScene : public cocos2d::Layer
	{
	public:
		static cocos2d::Scene* scene();

	public:
		bool init() override;

		CREATE_FUNC(JukeboxScene);

	protected:
		JukeboxScene();
		void setSelected(int selectedIndex);
		void onKeyDown(EventKeyboard::KeyCode keyCode, cocos2d::Event *e);

	protected:
		Jukebox* mJukebox;
		cocos2d::Sprite* mTitleImage;
		cocos2d::LabelTTF* mTitleLabel;
		int mSelectedIndex;
	};

}