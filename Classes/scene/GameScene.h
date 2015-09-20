/*
 * Game
 * ���� �÷��̸� ����ϴ� Ŭ����.
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
			// ��Ʈ�� �������� ����ߴ��� üũ
			return getNotePosit(note, bpm, rate, playTime, posit) <= 0;
		}
		double getNotePosit(const BmsNote *note, int bpm, double rate, INT64 playTime, int posit)
		{
			// ����κ��� ������� �ð����� ��ġ�� ã����(����: playTime)
			return ( note->meterPosition() * SINK1 * rate / bpm - (int)playTime );
		}
		double getMonitorPosit(const BmsNote *note, int bpm, double rate, INT64 playTime, int posit)
		{
			// �ð����� ��ġ�� ȭ����ǥ�� ��ȯ
			return getNotePosit(note, bpm, rate, playTime, posit) * bpm * mSpeed / (1<<SINK2);
		}

		SongInfo *mSong;			// �뷡

		float mStartTime;			// ���� ������ �ð�
		float mEndPosition;		// ������ ��ġ
		int mCombo;				// �޺�
		int mScore;				// ����
		bool bMiss;				// �̽�����(missBga �׸����)
		bool bAuto;				// ���俩��
		float mCurrentBpm;		// bpm
		int nowMadi;			// ����Ȯ��� ����
		float mSpeed;				// ���
		double mBarScale;		// ������(�Ϲ�:1.0)
		float mLastBpmTime;		// �ֱ� bpm�� ���� �ð�
		float mLastBpmMeter;	// �ֱ� bpm�� ���� ����

		//�׸���� ������
		int origMadi, origPosit;
		float origTime;
		float mPosition;

		// bms ����ü
		BmsDocument mBmsDocument;
		std::map<std::string, ChannelPlayer*> mChannelPlayers;

		// Ű ���ε�
		std::map<cocos2d::EventKeyboard::KeyCode, Key::MappedKey> mKeyMap;
		std::map<Key::MappedKey, Key::KeyState> mKeyState;

		// ���̺귯�� ��
		std::map<std::string, cocos2d::Image*> mImageDictionary;		// for async loading
		std::map<std::string, cocos2d::Texture2D*> mBgaDictionary;	// texture version of mImageDictionary
		std::map<std::string, Audio::Wave*> mWavDictionary;
	};

}

#endif // #ifndef __GAME_H__
