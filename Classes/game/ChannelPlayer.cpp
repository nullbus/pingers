#include "ChannelPlayer.h"
#include "bms/BmsChannel.h"
#include "bms/BmsNote.h"
#include "definitions.h"
#include "audio/Sound.h"
#include "event/BpmEvent.h"
#include "event/BarScaleEvent.h"
#include "event/JudgeEvent.h"
#include "event/KeyEvent.h"
#include "util/logutil.h"
#include "util/timeutil.h"
using namespace cocos2d;

namespace BMS
{
	BpmChannelPlayer::BpmChannelPlayer(const BmsChannel* channel):
		ChannelPlayer(channel)
	{

	}


	void BpmChannelPlayer::playNote(const BmsNote* note)
	{
		if( !note )
			return;

		float bpm = strtol(note->value().c_str(), NULL, 16);
		Log::d("bpm changed to %.1f", bpm);
		Event::BpmEvent event(bpm, note->meterPosition(), note->drawPosition());
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Event::BpmEvent::CHANGED, &event);
	}


	WaveChannelPlayer::WaveChannelPlayer(const BmsChannel* channel, const std::map<std::string, Audio::Sound*>& library):
		LibraryChannelPlayer(channel, library)
	{

	}


	void WaveChannelPlayer::playNote(const BmsNote* note)
	{
		auto wave = mLibrary.find(note->value());
		if(wave != mLibrary.end() && wave->second)
		{
			Log::d("play wave %s(%.3f)", note->value().c_str(), note->meterPosition());
			wave->second->play();
		}
	}


	BgaChannelPlayer::BgaChannelPlayer(const BmsChannel* channel, const std::map<std::string, cocos2d::Texture2D*>& library, bool isMiss):
		LibraryChannelPlayer(channel, library),
		mSprite(Sprite::create())
	{
		if(isMiss) {
			Director::getInstance()->getEventDispatcher()->addCustomEventListener(Event::JudgeEvent::JUDGE, CC_CALLBACK_1(BgaChannelPlayer::onJudge, this));
		}
	}


	void BgaChannelPlayer::playNote(const BmsNote* note)
	{
		auto bga = mLibrary.find(note->value());
		if(bga != mLibrary.end() && bga->second)
		{
			Texture2D* texture = bga->second;
			mSprite->setTexture(texture);
			mSprite->setVisible(true);

			// preserves current width/height
			const Rect rect = CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, texture->getPixelsWide(), texture->getPixelsHigh()));
			const Size size = mSprite->getContentSize();
			mSprite->setTextureRect(rect, false, size);
			mSprite->setScaleX(size.width / rect.size.width);
			mSprite->setScaleY(size.height / rect.size.height);
		}
		else
		{
			mSprite->setTexture(NULL);
			mSprite->setVisible(false);
		}
	}

	void BgaChannelPlayer::onJudge(EventCustom* e)
	{
		Event::JudgeEvent* judgeEvent = static_cast<Event::JudgeEvent*>(e->getUserData());
		if(judgeEvent->judge() == JUDGE_FAIL)
		{
			mSprite->setVisible(true);

			// invisible after 1 sec
			mSprite->unschedule(schedule_selector(BgaChannelPlayer::onTimer));
			mSprite->scheduleOnce(schedule_selector(BgaChannelPlayer::onTimer), 1);
		}
	}

	void BgaChannelPlayer::onTimer(float delay)
	{
		mSprite->setVisible(false);
	}

	Sprite* BgaChannelPlayer::getSprite()
	{
		return mSprite;
	}


	PlayChannelPlayer::PlayChannelPlayer(const BmsChannel* channel, const std::map<std::string, Audio::Sound*>& library, Key::MappedKey key, bool autoplay /* = false */) : 
		WaveChannelPlayer(channel, library),
		mAutoPlay(autoplay),
		mKey(key)
	{
		Director::getInstance()->getEventDispatcher()->addCustomEventListener(Event::KeyEvent::DOWN, CC_CALLBACK_1(PlayChannelPlayer::onKeydown, this));
	}


	void PlayChannelPlayer::onKeydown(EventCustom* event)
	{
		Event::KeyEvent* keyEvent = static_cast<Event::KeyEvent*>(event->getUserData());
		if(keyEvent && keyEvent->keyCode() == mKey)
		{
			if(mProcessed && mProcessed >= mChannel->notes().size())
			{
				playNote(mChannel->notes()[mChannel->notes().size()-1]);
			}
			else if(mProcessed == 0)
			{
				if(mChannel->notes().size())
					playNote( mChannel->notes()[0] );
			}
			else
			{
				const BmsNote* prevNote = mChannel->notes()[mProcessed-1];
				const BmsNote* nextNote = mChannel->notes()[mProcessed];
				if(abs(prevNote->meterPosition() - keyEvent->pos()) < abs(nextNote->meterPosition() - keyEvent->pos()))
				{
					playNote(prevNote);
				}
				else
				{
					playNote(nextNote);
				}
			}
			// TODO judge it!

			if(mProcessed < mChannel->notes().size())
			{
				const BmsNote* curNote = mChannel->notes()[mProcessed];
				float diff = curNote->meterPosition() - keyEvent->pos();
				if(abs(diff) < 0.05)
				{
					Log::d("[Judge] KOOL %d", mKey);
					processNoteJudge(JUDGE_KOOL);
				}
				else if(abs(diff) < 0.1)
				{
					Log::d("[Judge] COOL %d", mKey);
					processNoteJudge(JUDGE_COOL);
				}
				else if(abs(diff) < 0.2)
				{
					Log::d("[Judge] GOOD %d", mKey);
					processNoteJudge(JUDGE_GOOD);
				}
				else if(abs(diff) < 0.3 || diff < 0)
				{
					Log::d("[Judge] MISS %d", mKey);
					processNoteJudge(JUDGE_MISS2);
				}
				else
				{
					Log::d("[Judge] nothing, %d, %f", mKey, diff);
				}
			}
		}
	}


	void PlayChannelPlayer::onKeyup(EventCustom* event)
	{
		// TODO if longnote, judge it
	}


	void PlayChannelPlayer::autoplay(float pos)
	{
		if(mAutoPlay)
		{
			for(size_t i=mProcessed; i < mChannel->notes().size(); i++)
			{
				const BmsNote* note = mChannel->notes()[i];
				if(!note)
					continue;

				if(note->drawPosition() < pos)
				{
					if(mAutoPlay)
					{
						Event::JudgeEvent judgeEvent(JUDGE_KOOL, mKey);
						Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Event::JudgeEvent::JUDGE, &judgeEvent);
					}

					playNote(note);
					mProcessed++;
				}
				else break;
			}
		}
		else
		{
			// TODO if failed to process, emit failed event
			if(0)
			{
				processNoteJudge(JUDGE_FAIL);
			}
		}
	}


	void PlayChannelPlayer::processNoteJudge(JUDGE judge)
	{
		Event::JudgeEvent judgeEvent(judge, mKey);
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Event::JudgeEvent::JUDGE, &judgeEvent);
		mProcessed++;
	}

}