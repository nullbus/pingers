#include "PlayNoteRenderer.h"
#include "game/ChannelPlayer.h"
#include "scene/GameScene.h"
#include "event/BpmEvent.h"
#include "event/GameConfigEvent.h"
#include "event/JudgeEvent.h"
#include "event/SpeedEvent.h"
#include "util/timeutil.h"
#include "util/logutil.h"
using namespace cocos2d;

namespace BMS
{
	PlayNoteRenderer::PlayNoteRenderer(const PlayChannelPlayer* player, Sprite* baseSprite):
		mChannelPlayer(player),
		mBaseSprite(baseSprite)
	{
		scheduleUpdate();
		autorelease();

		auto dispatcher = Director::getInstance()->getEventDispatcher();
		dispatcher->addCustomEventListener(Event::BpmEvent::CHANGED, std::bind(&PlayNoteRenderer::onBpmChanged, this, std::placeholders::_1));
		dispatcher->addCustomEventListener(Event::JudgeEvent::JUDGE, std::bind(&PlayNoteRenderer::onJudge, this, std::placeholders::_1));
		dispatcher->addCustomEventListener(Event::SpeedEvent::CHANGED, std::bind(&PlayNoteRenderer::onSpeedChanged, this, std::placeholders::_1));

		mBaseSprite->retain();
		setContentSize(Size(mBaseSprite->getContentSize().width, 0));
	}

	PlayNoteRenderer::~PlayNoteRenderer()
	{
		mBaseSprite->release();
	}

	void PlayNoteRenderer::update(float delta)
	{
		Node::update(delta);

		if(mChannelPlayer == NULL)
			return;

		float currentMeter = mBpmChangedMeter + calculateMeter(mCurrentBpm, currentTime() - mBpmChangedTime);

		for(int i=mChannelPlayer->numProcessedNotes()-1 ; i >= 0; i--)
		{
			if(!mNoteSprites[i]->isVisible())
				break;

			mNoteSprites[i]->setVisible(false);
		}

		for(unsigned i=mChannelPlayer->numProcessedNotes() ; i < mChannelPlayer->getChannel()->notes().size(); i++)
		{
			const BmsNote* note = mChannelPlayer->getChannel()->notes()[i];
			float drawPosition = (note->drawPosition() - currentMeter) * mOriginalBpm * mSpeed;
			mNoteSprites[i]->setPositionY(drawPosition);
		}
	}

	void PlayNoteRenderer::reset(Event::GameConfigEvent* configEvent)
	{
		removeAllChildren();

		mStartTime = mBpmChangedTime = configEvent->startTime();
		mCurrentBpm = mOriginalBpm = configEvent->songInfo().getBpm();
		mSpeed = configEvent->speed();
		mBpmChangedMeter = 0;

		// if no notes in channel then mChannelPlayer can be null
		if(mChannelPlayer == NULL)
			return;

		int length = mChannelPlayer->getChannel()->notes().size();
		mNoteSprites.swap(std::vector<Node*>(length));

		for(int i=0; i<length; i++)
		{
			const BmsNote* note = mChannelPlayer->getChannel()->notes()[i];
			Sprite* spriteNote = Sprite::createWithTexture(mBaseSprite->getTexture(), mBaseSprite->getTextureRect());
			spriteNote->setAnchorPoint(Point::ZERO);
			mNoteSprites[i] = spriteNote;
			addChild(spriteNote);
		}
	}

	void PlayNoteRenderer::onBpmChanged(EventCustom* event)
	{
		Event::BpmEvent* bpmEvent = static_cast<Event::BpmEvent*>(event->getUserData());
		mCurrentBpm = bpmEvent->bpm();
		mBpmChangedTime = bpmEvent->timestamp();
		mBpmChangedMeter = bpmEvent->drawPosition();
	}

	void PlayNoteRenderer::onJudge(EventCustom* event)
	{
		Event::JudgeEvent* judgeEvent = static_cast<Event::JudgeEvent*>(event->getUserData());
		// TODO flare effect
	}

	void PlayNoteRenderer::onSpeedChanged(EventCustom* event)
	{
		Event::SpeedEvent* speedEvent = static_cast<Event::SpeedEvent*>(event->getUserData());
		mSpeed = speedEvent->speed();
	}
}