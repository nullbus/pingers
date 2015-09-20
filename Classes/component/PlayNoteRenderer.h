#pragma once
#include <cocos2d.h>
#include <vector>

namespace BMS
{
	class PlayChannelPlayer;

	namespace Event
	{
		class GameConfigEvent;
	}

	class PlayNoteRenderer : public cocos2d::Node
	{
	public:
		PlayNoteRenderer(const PlayChannelPlayer* player, cocos2d::Sprite* baseSprite);
		virtual ~PlayNoteRenderer();

		void reset(Event::GameConfigEvent* event);

	protected:
		void update(float delta) override;
		void onBpmChanged(cocos2d::EventCustom* event);
		void onJudge(cocos2d::EventCustom* event);
		void onSpeedChanged(cocos2d::EventCustom* event);

	protected:
		const PlayChannelPlayer* mChannelPlayer;
		cocos2d::Sprite* mBaseSprite;
		std::vector<cocos2d::Node*> mNoteSprites;

		float mOriginalBpm;
		float mCurrentBpm;
		float mStartTime;
		float mSpeed;
		float mBpmChangedTime;
		float mBpmChangedMeter;
	};

}