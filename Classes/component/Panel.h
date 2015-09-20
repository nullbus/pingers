#pragma once
#include <cocos2d.h>
#include <map>
#include <string>
#include "definitions.h"
#include "enums.h"

namespace BMS
{
	class ChannelPlayer;
	typedef std::map<Key::MappedKey, cocos2d::Sprite*> KeySpriteMap;

	// most super class Panel
	class Panel : public cocos2d::Node
	{
	public:
		static const char* SKIN_PATH;

	protected:
		// must subclass to create Panel instance
		Panel();

		// listeners
		virtual void onJudge(cocos2d::EventCustom* event) {}
		virtual void onKeyDown(cocos2d::EventCustom* event) {}
		virtual void onKeyUp(cocos2d::EventCustom* event) {}
		virtual void onSpeedChange(cocos2d::EventCustom* event) {}
		virtual void onReset(cocos2d::EventCustom* event) {}
	};


	class BackPanel : public Panel
	{
	public:
		BackPanel(const std::map<std::string, ChannelPlayer*>& players);

	protected:
		virtual void onKeyDown(cocos2d::EventCustom* event) override;
		virtual void onKeyUp(cocos2d::EventCustom* event) override;
		virtual void onReset(cocos2d::EventCustom* event) override;
		virtual void onJudge(cocos2d::EventCustom* event) override;

	protected:
		KeySpriteMap mLightSprites;
		const std::map<std::string, ChannelPlayer*>& mChannelPlayers;
	};

	class FrontPanel : public Panel
	{
	public:
		FrontPanel();

	protected:
		virtual void onKeyDown(cocos2d::EventCustom* event) override;
		virtual void onKeyUp(cocos2d::EventCustom* event) override;
		virtual void onReset(cocos2d::EventCustom* event) override;

	protected:
		KeySpriteMap mButtonPress;
	};

	class ControlPanel : public Panel
	{
	public:
		ControlPanel();

	protected:
		virtual void onSpeedChange(cocos2d::EventCustom* event) override;
		virtual void onReset(cocos2d::EventCustom* event) override;
	};
}