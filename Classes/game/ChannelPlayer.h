#pragma once
#include <cocos2d.h>
#include <map>
#include <string>
#include "bms/BmsChannel.h"
#include "bms/BmsNote.h"
#include "definitions.h"
#include "enums.h"

namespace BMS
{
	namespace Audio
	{
		class Sound;
	}

	class ChannelPlayer : public cocos2d::Object
	{
	public:
		ChannelPlayer(const BmsChannel* channel) : cocos2d::Object(), mChannel(channel), mProcessed(0) {}

		virtual void autoplay(float pos)
		{
			for(size_t i=mProcessed; i < mChannel->notes().size(); i++)
			{
				const BmsNote* note = mChannel->notes()[i];
				if(!note)
					continue;

				if(note->drawPosition() < pos)
				{
					playNote(note);
					mProcessed++;
				}
				else break;
			}
		}

		virtual const BmsNote* getNearNote(float pos) const
		{
			// FIXME: implement and remove this assert
			assert(false);

			return NULL;
		}

		virtual int numProcessedNotes() const
		{
			return mProcessed;
		}

		virtual const BmsChannel* getChannel() const
		{
			return mChannel;
		}

		virtual void playNote(const BmsNote* note) = 0;

	protected:
		const BmsChannel* mChannel;
		unsigned mProcessed;
	};


	///
	class BpmChannelPlayer : public ChannelPlayer
	{
	public:
		BpmChannelPlayer(const BmsChannel* channel);
		virtual void playNote(const BmsNote* note);
	};


	///
	template <typename Libtype>
	class LibraryChannelPlayer : public ChannelPlayer
	{
	public:
		LibraryChannelPlayer(const BmsChannel* channel, const std::map<std::string, Libtype>& library):
			ChannelPlayer(channel),
			mLibrary(library)
		{
		}

	protected:
		const std::map<std::string, Libtype>& mLibrary;
	};


	///
	class WaveChannelPlayer : public LibraryChannelPlayer<Audio::Sound*>
	{
	public:
		WaveChannelPlayer(const BmsChannel* channel, const std::map<std::string, Audio::Sound*>& library);
		virtual void playNote(const BmsNote* note);
	};


	///
	class BgaChannelPlayer : public LibraryChannelPlayer<cocos2d::Texture2D*> 
	{
	public:
		BgaChannelPlayer(const BmsChannel* channel, const std::map<std::string, cocos2d::Texture2D*>& library, bool isMiss);
		virtual void playNote(const BmsNote* note);
		cocos2d::Sprite* getSprite();

	protected:
		void onJudge(cocos2d::EventCustom* event);
		void onTimer(float delay);

	protected:
		cocos2d::Sprite* mSprite;
	};


	///
	class PlayChannelPlayer : public WaveChannelPlayer
	{
	public:
		PlayChannelPlayer(const BmsChannel* channel, const std::map<std::string, Audio::Sound*>& library, Key::MappedKey key, bool autoplay = false);
		virtual void autoplay(float pos) override;

	protected:
		void onKeydown(cocos2d::EventCustom* keyEvent);
		void onKeyup(cocos2d::EventCustom* keyEvent);
		void processNoteJudge(JUDGE judge);

	protected:
		bool mAutoPlay;
		Key::MappedKey mKey;
	};

}