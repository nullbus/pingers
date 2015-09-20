#include "JukeboxScene.h"
#include "game/Song.h"
#include "game/Jukebox.h"
#include "LoadingScene.h"
#include "util/logutil.h"
#include <sstream>

using namespace cocos2d;

namespace BMS
{
	JukeboxScene::JukeboxScene() :
		mJukebox(NULL),
		mTitleImage(NULL),
		mSelectedIndex(0)
	{}

	Scene* JukeboxScene::scene()
	{
		Scene* scene = Scene::create();
		Layer* layer = JukeboxScene::create();
		if(scene && layer)
			scene->addChild(layer);

		return scene;
	}

	bool JukeboxScene::init()
	{
		mJukebox = Jukebox::getInstance();
		if( !mJukebox )
			return false;

		if( !mJukebox->loadJukebox() )
			return false;

		mTitleImage = Sprite::create();
		mTitleImage->setPosition(Point(300, 150));
		mTitleImage->setContentSize(Size(2000, 1500));
		mTitleImage->setAnchorPoint(Point(0, 0));
		addChild(mTitleImage);

		mTitleLabel = LabelTTF::create("", "Thonburi", 20);
		mTitleLabel->setPosition(Point(100, 100));
		mTitleLabel->setHorizontalAlignment(TextHAlignment::LEFT);
		mTitleLabel->setAnchorPoint(Point(0, 0));
		addChild(mTitleLabel);

		setSelected(0);

		auto keyListener = cocos2d::EventListenerKeyboard::create();
		keyListener->onKeyPressed = std::bind(&JukeboxScene::onKeyDown, this, std::placeholders::_1, std::placeholders::_2);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(keyListener, 1);
		return true;
	}

	void JukeboxScene::onKeyDown(EventKeyboard::KeyCode keyCode, Event *e)
	{
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			setSelected(mSelectedIndex - 1);
			break;

		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			setSelected(mSelectedIndex + 1);
			break;

		case EventKeyboard::KeyCode::KEY_RETURN:
		{
			SongInfo* song = mJukebox->getSong(mSelectedIndex);
			Director::getInstance()->pushScene(LoadingScene::scene(song));
		}
			break;
		}
	}

	void JukeboxScene::setSelected(int selectedIndex)
	{
		if(selectedIndex < 0)
			selectedIndex = mJukebox->getSongSize()-1;
		if(selectedIndex >= mJukebox->getSongSize())
			selectedIndex = 0;

		mSelectedIndex = selectedIndex;
		Log::d("selected index: %d", mSelectedIndex);

		SongInfo* song = mJukebox->getSong(mSelectedIndex);
		mTitleImage->setVisible(false);

		if(song->getStageFile() != "")
		{
			Texture2D* texture = Director::getInstance()->getTextureCache()->addImage((song->getParentPath() + song->getStageFile()).c_str());
			mTitleImage->setTexture(texture);
			if(texture)
			{
				mTitleImage->setTextureRect(Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));
				mTitleImage->setVisible(true);
			}
		}

		std::ostringstream result;
		result << "Name: " << song->getTitle() << std::endl;
		result << "Artist: " << song->getArtist() << std::endl;
		result << "PlayLevel: " << song->getLevel() << std::endl;

		switch(song->getNumKey()){
		case KEY_5:
			result << "Key: 5";
			break;
		case KEY_7:
			result << "Key: 7";
			break;
		case KEY_10:
			result << "Key: 10";
			break;
		case KEY_14:
			result << "Key: 14";
			break;
		}

		mTitleLabel->setString(result.str().c_str());
	}
}