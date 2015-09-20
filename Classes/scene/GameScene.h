/*
 * Game
 * 게임 플레이를 담당하는 클래스.
 */

#ifndef __GAME_H__
#define __GAME_H__

#include <string>
#include <cocos2d.h>
#include <vector>
#include <map>
#include "bms/BmsDocument.h"
#include "bms/BmsNote.h"
#include "enums.h"
#include "game/ChannelPlayer.h"

#pragma warning(disable:4018)
#pragma warning(disable:4244)

namespace BMS
{
	namespace Audio
	{
		class Wave;
	}

	class SongInfo;
	class BmsChannel;

	class GameScene : public cocos2d::Layer
	{
	public:
		static GameScene* create(SongInfo* song);

	public:
		GameScene(SongInfo* song);
		virtual ~GameScene();

		virtual void update(float delta) override;
		virtual bool init() override;

		bool loadGame();

	protected:
		void onKeyDown(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e);
		void onKeyUp(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e);

		void setSpeed(float newSpeed);
		void start(float dt);
		bool checkGameEnd();
		void gameEnd();
		void onJudge(cocos2d::EventCustom* event);
		void onBpmChanged(cocos2d::EventCustom* event);

		bool isPassed(const BmsNote *note, int bpm, double rate, INT64 playTime, int posit)
		{
			// 노트가 판정선을 통과했는지 체크
			return getNotePosit(note, bpm, rate, playTime, posit) <= 0;
		}
		double getNotePosit(const BmsNote *note, int bpm, double rate, INT64 playTime, int posit)
		{
			// 현재로부터 상대적인 시간상의 위치를 찾아줌(기준: playTime)
			return ( note->meterPosition() * SINK1 * rate / bpm - (int)playTime );
		}
		double getMonitorPosit(const BmsNote *note, int bpm, double rate, INT64 playTime, int posit)
		{
			// 시간상의 위치를 화면좌표로 변환
			return getNotePosit(note, bpm, rate, playTime, posit) * bpm * mSpeed / (1<<SINK2);
		}

		SongInfo *mSong;			// 노래

		float mStartTime;			// 게임 시작한 시간
		float mEndPosition;		// 끝내는 위치
		int mCombo;				// 콤보
		int mScore;				// 점수
		bool bMiss;				// 미스여부(missBga 그리기용)
		bool bAuto;				// 오토여부
		float mCurrentBpm;		// bpm
		int nowMadi;			// 마디확장시 마디
		float mSpeed;				// 배속
		double mBarScale;		// 마디폭(일반:1.0)
		float mLastBpmTime;		// 최근 bpm이 변한 시간
		float mLastBpmMeter;	// 최근 bpm이 변한 마디

		//그리기용 기준점
		int origMadi, origPosit;
		float origTime;
		float mPosition;

		// bms 구조체
		BmsDocument mBmsDocument;
		std::map<std::string, ChannelPlayer*> mChannelPlayers;

		// 키 바인딩
		std::map<cocos2d::EventKeyboard::KeyCode, Key::MappedKey> mKeyMap;
		std::map<Key::MappedKey, Key::KeyState> mKeyState;

		// 라이브러리 맵
		std::map<std::string, cocos2d::Image*> mImageDictionary;		// for async loading
		std::map<std::string, cocos2d::Texture2D*> mBgaDictionary;	// texture version of mImageDictionary
		std::map<std::string, Audio::Wave*> mWavDictionary;
	};

}

#endif // #ifndef __GAME_H__
