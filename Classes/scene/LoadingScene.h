#pragma once
#include <cocos2d.h>

namespace BMS
{
	class SongInfo;
	class GameScene;

	class LoadingScene : public cocos2d::Layer
	{
	public:
		static cocos2d::Scene* scene(SongInfo* song);
		static LoadingScene* create(SongInfo* song);

	public:
		LoadingScene(SongInfo *song);
		virtual ~LoadingScene();

		virtual bool init() override;
		virtual void update(float delta) override;

		void onKeyDown(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* e);
		void startLoading();

	private:
		volatile bool m_bFinish;
		volatile bool m_bFailed;

		SongInfo *mSong;
		GameScene* mGameScene;
	};
}
