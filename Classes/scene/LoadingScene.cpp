#include "LoadingScene.h"
#include "GameScene.h"
#include <Windows.h>
#include "JukeboxScene.h"
using namespace cocos2d;

namespace BMS
{
	static DWORD WINAPI LoadStart(LPVOID pointerToLoadingScene)
	{
		LoadingScene *pThis = static_cast<LoadingScene*>( pointerToLoadingScene );
		pThis->startLoading();
		return 0;
	}


	Scene* LoadingScene::scene(SongInfo* song)
	{
		Scene* scene = Scene::create();
		LoadingScene* layer = LoadingScene::create(song);
		if(scene && layer)
		{
			layer->scheduleUpdate();
			scene->addChild(layer);
		}

		return scene;
	}


	LoadingScene* LoadingScene::create(SongInfo* song)
	{
		LoadingScene *pRet = new LoadingScene(song);
		if (pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			pRet = NULL;
			return NULL;
		}
	}


	LoadingScene::LoadingScene(SongInfo *song)
		:m_bFinish(false),
		m_bFailed(false),
		mSong(song),
		mGameScene(NULL)
	{}


	bool LoadingScene::init()
	{
		mGameScene = GameScene::create(mSong);

		DWORD tid;
		HANDLE hThread = CreateThread(NULL, 0, LoadStart, this, 0, &tid);
		if(NULL == hThread)
			return false;

		Sprite* stageImage = Sprite::create((mSong->getParentPath() + mSong->getStageFile()).c_str());
		if(stageImage)
		{
			const Size& screenSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
			stageImage->setScaleX(screenSize.width / stageImage->getContentSize().width);
			stageImage->setScaleY(screenSize.height / stageImage->getContentSize().height);
			stageImage->setAnchorPoint(Point::ZERO);
			addChild(stageImage);
		}

		CloseHandle(hThread);

		auto keyListener = EventListenerKeyboard::create();
		keyListener->onKeyPressed = CC_CALLBACK_2(LoadingScene::onKeyDown, this);
		Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);

		return true;
	}


	void LoadingScene::update(float dt)
	{
		if(m_bFinish)
		{
			Scene* scene = Scene::create();
			if(scene && mGameScene)
				scene->addChild(mGameScene);

			Director::getInstance()->pushScene(scene);
		}
		else if(m_bFailed)
		{
			// go to jukebox
			Director::getInstance()->popScene();
		}
	}


	void LoadingScene::onKeyDown(EventKeyboard::KeyCode keyCode, Event* e)
	{
		if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			m_bFailed = true;
		}
	}


	void LoadingScene::startLoading()
	{
		m_bFinish = false;

		// start loading
		mGameScene->loadGame();

		m_bFinish = true;
	}

}