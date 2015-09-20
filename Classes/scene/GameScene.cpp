#include "GameScene.h"
#include <windows.h>
#include <algorithm>
#include "bms/BmsChannel.h"
#include "bms/BmsParser.h"
#include "component/Panel.h"
#include "event/BpmEvent.h"
#include "game/Song.h"
#include "game/ChannelPlayer.h"
#include "event/ComboEvent.h"
#include "event/GameConfigEvent.h"
#include "event/JudgeEvent.h"
#include "event/KeyEvent.h"
#include "event/SpeedEvent.h"
#include "audio/AudioManager.h"
#include "util/stringutil.h"
#include "util/timeutil.h"
#include "util/containerutil.h"
#include "JukeboxScene.h"
using namespace cocos2d;

#pragma warning(disable:4996)
#pragma warning(disable:4101)

namespace BMS
{
	namespace
	{
		const char keyChannelMap[][3] = {
			"16", "11", "12", "13", "14", "15", "17", "18",		// 1p
			"26", "21", "22", "23", "24", "25", "27", "28"		// 2p
		};
	}

/*
	static CCImage::EImageFormat computeImageFormatType(string& filename)
	{
		CCImage::EImageFormat ret = CCImage::kFmtUnKnown;

		if ((std::string::npos != filename.find(".jpg")) || (std::string::npos != filename.find(".jpeg")))
		{
			ret = CCImage::kFmtJpg;
		}
		else if ((std::string::npos != filename.find(".png")) || (std::string::npos != filename.find(".PNG")))
		{
			ret = CCImage::kFmtPng;
		}
		else if ((std::string::npos != filename.find(".tiff")) || (std::string::npos != filename.find(".TIFF")))
		{
			ret = CCImage::kFmtTiff;
		}
		else if ((std::string::npos != filename.find(".bmp")) || (std::string::npos != filename.find(".BMP")))
		{
			ret = CCImage::kFmtBmp;
		}
#if (CC_TARGET_PLATFORM != CC_PLATFORM_WINRT) && (CC_TARGET_PLATFORM != CC_PLATFORM_WP8)
		else if ((std::string::npos != filename.find(".webp")) || (std::string::npos != filename.find(".WEBP")))
		{
			ret = CCImage::kFmtWebp;
		}
#endif

		return ret;
	}*/

	GameScene* GameScene::create(SongInfo* song)
	{
		GameScene* pRet = new GameScene(song);
		if(pRet && pRet->init())
		{
			pRet->autorelease();
			return pRet;
		}
		else
		{
			delete pRet;
			return NULL;
		}
	}

	bool GameScene::init()
	{
		if( !CCLayer::init() )
			return false;

#define KEY_MAP(name) EventKeyboard::KeyCode::KEY_##name
		mKeyMap[KEY_MAP(LEFT_SHIFT)] = Key::Player1_Scratch;
		mKeyMap[KEY_MAP(Z)] = Key::Player1_1;
		mKeyMap[KEY_MAP(S)] = Key::Player1_2;
		mKeyMap[KEY_MAP(X)] = Key::Player1_3;
		mKeyMap[KEY_MAP(D)] = Key::Player1_4;
		mKeyMap[KEY_MAP(C)] = Key::Player1_5;
		mKeyMap[KEY_MAP(F)] = Key::Player1_6;
		mKeyMap[KEY_MAP(V)] = Key::Player1_7;

		mKeyMap[KEY_MAP(RIGHT_SHIFT)] = Key::Player2_Scratch;
		mKeyMap[KEY_MAP(M)] = Key::Player2_1;
		mKeyMap[KEY_MAP(K)] = Key::Player2_2;
		mKeyMap[KEY_MAP(COMMA)] = Key::Player2_3;
		mKeyMap[KEY_MAP(L)] = Key::Player2_4;
		mKeyMap[KEY_MAP(PERIOD)] = Key::Player2_5;
		mKeyMap[KEY_MAP(SEMICOLON)] = Key::Player2_6;
		mKeyMap[KEY_MAP(SLASH)] = Key::Player2_7;

		mKeyMap[KEY_MAP(F3)] = Key::SpeedDown;
		mKeyMap[KEY_MAP(F4)] = Key::SpeedUp;
#undef KEY_MAP

		for(auto it = mKeyMap.begin(); it != mKeyMap.end(); it++)
		{
			mKeyState[it->second] = Key::KEY_UP;
		}

		scheduleOnce(schedule_selector(GameScene::start), .0f);
		scheduleUpdate();

		auto keyListener = EventListenerKeyboard::create();
		keyListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyDown, this);
		keyListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyUp, this);

		auto dispatcher = Director::getInstance()->getEventDispatcher();

		dispatcher->addEventListenerWithSceneGraphPriority(keyListener, this);
		dispatcher->addCustomEventListener(Event::BpmEvent::CHANGED, CC_CALLBACK_1(GameScene::onBpmChanged, this));
		dispatcher->addCustomEventListener(Event::JudgeEvent::JUDGE, CC_CALLBACK_1(GameScene::onJudge, this));

		return true;
	}


	GameScene::GameScene(SongInfo* song):
		CCLayer(),
		mSong(song),
		mSpeed(1.0f)
	{
	}

	GameScene::~GameScene()
	{
		removeAllChildrenWithCleanup(true);

		// clear
		clearPointerMap(mImageDictionary);
		clearPointerMap(mBgaDictionary);
		clearPointerMap(mWavDictionary);
		clearPointerMap(mChannelPlayers);
	}


	void GameScene::update(float dt)
	{
		float playTime = currentTime() - mStartTime; // - origTime ??
		float position = mLastBpmMeter + calculateMeter(mCurrentBpm, currentTime() - mLastBpmTime);
		if( checkGameEnd() )
		{
			gameEnd();
			return;
		}

		for(auto it = mChannelPlayers.begin(); it != mChannelPlayers.end(); it++)
		{
			if(it->second)
				it->second->autoplay(position);
		}

		/*
		float rate;
		char buf[16];

		speed = panel->getSpeed();
		bmsWave->update();

		sprintf(buf, "%d, %.1f", noteIndex, mCurrentBpm);
		g->drawString(Panel::PANEL_WIDTH+100, 100, buf);

		int i = (int)playTime*mCurrentBpm/(SINK1*250*nowRate);

		// 현재마디 구하기
		int madi = (i>>2) + origMadi;

		// 해당 마디의 박자(4박)
		i &= 3;

		int sink2 = 16;
		FRECT rt;
		Note *n;

		//bpm노트처리
		while(bpmNote.size() != bpmNoteIndex){

			n = &bpmNote[bpmNoteIndex];

			if(isPassed(n, mCurrentBpm, nowRate, playTime, origPosit) == false)
				break;

			// origPosit은 절대값
			origPosit = n->madi + n->madiPosit;
			origMadi = madi;
			origTime = origTime + playTime;
			playTime = 0;
			mCurrentBpm = n->wav*10;
			bpmNoteIndex++;
		}

		// 마디노트처리
		while(madiNote.size() != madiNoteIndex){
			n = &madiNote[ madiNoteIndex ];
			if(isPassed(n, mCurrentBpm, nowRate, playTime, origPosit) == false)
				break;
			origMadi = nowMadi = n->madi/1000;
			origTime = (origTime + playTime);
			origPosit = n->madi;
			nowRate = getMadiRate(nowMadi);
			playTime = 0;
			madiNoteIndex++;
		}

		// BGA 그리기
		while(bgaNote.size() != bgaNoteIndex){
			n = &bgaNote[ bgaNoteIndex ];
			if(isPassed(n, mCurrentBpm, nowRate, playTime, origPosit))
				bgaNoteIndex++;
			else
				break;
		}
		if(bgaNote.size() && bgaNoteIndex){
			DoBmsImg *bitmap = bga->getBga(bgaNote[bgaNoteIndex-1].wav-1);
			if(bitmap){
				SetFRect(&rt, 400, 150, 400+bitmap->getWidth(), 150+bitmap->getHeight());
				g->drawImage(bitmap, RS_NORMAL, &rt);
			}
		}

		// Masked BGA 그리기
		while(bgaMaskNote.size() != bgaMaskNoteIndex){
			n = &bgaMaskNote[ bgaMaskNoteIndex ];
			if(isPassed(n, mCurrentBpm, nowRate, playTime, origPosit))
				bgaMaskNoteIndex++;
			else
				break;
		}
		if(bgaMaskNote.size() && bgaMaskNoteIndex){
			DoBmsImg *bitmap = bga->getBga(bgaMaskNote[bgaMaskNoteIndex-(bgaMaskNoteIndex==0?0:1)].wav-1);
			if(bitmap){
				SetFRect(&rt, 400, 150, 400+bitmap->getWidth(), 150+bitmap->getHeight());
				g->drawImage(bitmap, RS_COLORKEY, &rt);
			}
		}

		// Miss BGA 그리기
		while(bgaMissNote.size() != bgaMissIndex){
			n = &bgaMissNote[ bgaMissIndex ];
			if(isPassed(n, mCurrentBpm, nowRate, playTime, origPosit))
				bgaMissIndex++;
			else
				break;
		}
		if(bgaMissNote.size() && bgaMissIndex && bMiss){
			DoBmsImg *bitmap = bga->getBga(bgaMissNote[bgaMissIndex-(bgaMissIndex==0?0:1)].wav-1);
			if(bitmap){
				SetFRect(&rt, 400, 150, 400+bitmap->getWidth(), 150+bitmap->getHeight());
				g->drawImage(bitmap, RS_COLORKEY, &rt);
			}
		}

		// 판넬 그리기
		if(panel){
			panel->drawPanel(panel);
		}

		sprintf(buf, "%.1f, %d", nowRate, nowMadi);
		g->drawString(500, 50, buf);


		//선그리기
		int j=0, tempBNI=0, tempMNI=0, tempPosit=origPosit, tempBpm=mCurrentBpm, tempNY=0, tempNYTime=0;
		INT64 tempPlayTime = playTime;
		double tempRate = nowRate;
		while(lineNote.size() != lineNoteIndex+j){
			double rateY;
			n = &lineNote[ lineNoteIndex + j ];
			double nY = getNotePosit(n, tempBpm, tempRate, tempPlayTime, tempPosit);

			if(bpmNote.size() != bpmNoteIndex + tempBNI){
				n = &bpmNote[ bpmNoteIndex + tempBNI ];
				rateY = getNotePosit(n, tempBpm, tempRate, tempPlayTime, tempPosit);

				if(rateY < nY){
					rateY = getNotePosit(n, tempBpm, tempRate, tempPlayTime, tempPosit);
					tempNY = getMonitorPosit(n, tempBpm, tempRate, tempPlayTime, tempPosit) + tempNY;
					tempPosit = n->madi + n->madiPosit;
					tempPlayTime = 0;
					tempBpm = n->wav * 10;
					tempBNI++;
					continue;
				}
			}

			if(madiNote.size() != madiNoteIndex + tempMNI){
				n = &madiNote[ madiNoteIndex + tempMNI ];
				rateY = getNotePosit(n, tempBpm, tempRate, tempPlayTime, tempPosit);

				if(rateY < nY){
					tempNY = getMonitorPosit(n, tempBpm, tempRate, tempPlayTime, tempPosit) + tempNY;
					tempPosit = n->madi;
					tempPlayTime = 0;
					tempRate = (float)n->wav/1000;
					tempMNI++;
					continue;
				}
			}

			n = &lineNote[ lineNoteIndex + j ];
			if(isPassed(n, mCurrentBpm, nowRate, playTime, origPosit)){
				lineNoteIndex++;
				continue;
			}else if(getMonitorPosit(n, tempBpm, tempRate, tempPlayTime, tempPosit) + tempNY > Panel::PANEL_HEIGHT){
				break;
			}else{
				sprintf(buf, "%d", madi+j);
				nY = Panel::PANEL_HEIGHT - getMonitorPosit(n, tempBpm, tempRate, tempPlayTime, tempPosit) - tempNY;
				g->drawLine(24, nY, Panel::PANEL_WIDTH, nY);
				g->drawString(Panel::PANEL_WIDTH+10, nY, buf);
			}

			j++;
		}

		// 플레이노트 그리기, 플레어 여부 체크
		for(int i=0; i<MAX_KEY; i++){

			j=0, tempBNI=0, tempMNI=0, tempPlayTime=playTime, tempPosit=origPosit, tempRate=nowRate, tempBpm=mCurrentBpm, tempNY=0, tempNYTime=0;
			while(playNote[i].size() > noteIndex[i] + j){
				double rateY;
				n = &playNote[i][ noteIndex[i] + j ];
				double nY = getNotePosit(n, tempBpm, tempRate, tempPlayTime, tempPosit);

				if(bpmNote.size() != bpmNoteIndex + tempBNI){
					n = &bpmNote[ bpmNoteIndex + tempBNI ];
					rateY = getNotePosit(n, tempBpm, tempRate, tempPlayTime, tempPosit);

					if(rateY < nY){
						tempNY = getMonitorPosit(n, tempBpm, tempRate, tempPlayTime, tempPosit) + tempNY;
						tempNYTime = getNotePosit(n, tempBpm, tempRate, tempPlayTime, tempPosit) + tempNYTime;
						tempPosit = n->madi + n->madiPosit;
						tempPlayTime = 0;
						tempBpm = n->wav * 10;
						tempBNI++;
						continue;
					}
				}

				if(madiNote.size() != madiNoteIndex + tempMNI){
					n = &madiNote[ madiNoteIndex + tempMNI ];
					rateY = getNotePosit(n, tempBpm, tempRate, tempPlayTime, tempPosit);

					if(rateY < nY){
						tempNY = getMonitorPosit(n, tempBpm, tempRate, tempPlayTime, tempPosit) + tempNY;
						tempNYTime = getNotePosit(n, tempBpm, tempRate, tempPlayTime, tempPosit) + tempNYTime;
						tempPosit = n->madi;
						tempPlayTime = 0;
						tempRate = (float)n->wav/1000;
						tempMNI++;
						continue;
					}
				}

				n = &playNote[i][ noteIndex[i] + j ];
				nY = getMonitorPosit(n, tempBpm, tempRate, tempPlayTime, tempPosit) + tempNY;
				if(nY > ((PlayNote*)n)->cposit)
					nY = nY;
				((PlayNote*)n)->cposit = nY;
				if(bAuto && isPassed(n, mCurrentBpm, nowRate, playTime, origPosit)){
					setPush(i);
					setUp(i);
					continue;
				}else if(getNotePosit(n, tempBpm, tempRate, tempPlayTime, tempPosit)+tempNYTime<-400){
					// 실패 처리
					panel->setJudge(JUDGE_FAIL);
					noteIndex[i]++;
					combo = 0;
					continue;
				}else if(nY <= Panel::PANEL_HEIGHT && nY >= 0){
					n->drawNote(g, Panel::PANEL_HEIGHT - nY, m_song->getBpm()/10.0);
				}else{
					break;
				}

				j++;
			}
		}

		panel->drawJudge(panel);
		panel->drawCombo(panel);

		sprintf(buf, "%d", score);
		g->drawString(200, 0, buf);

		// wav노트 처리
		while(wavNote.size() != wavNoteIndex){
			n = &wavNote[ wavNoteIndex ];

			if(!isPassed(n, mCurrentBpm, nowRate, playTime, origPosit))
				break;
			gameWav->playWave(bmsWave, n->wav-1);
			wavNoteIndex++;
		}

		// BG노트 처리
		while(notes.size() != bgNoteIndex){
			n = &notes[ bgNoteIndex ];

			if(!isPassed(n, mCurrentBpm, nowRate, playTime, origPosit))
				break;
			gameWav->playWave(bmsWave, n->wav-1);
			bgNoteIndex++;
		}*/
	}


	bool GameScene::checkGameEnd()
	{
		for(auto i = mBmsDocument.channel().begin(); i != mBmsDocument.channel().end(); i++)
		{
			if( i->second->endPosition() > mPosition)
				return false;
		}
		return true;
	}


	void GameScene::gameEnd()
	{
		CCDirector::sharedDirector()->replaceScene(JukeboxScene::scene());
	}


	void GameScene::onKeyDown(EventKeyboard::KeyCode keyCode, cocos2d::Event* e)
	{
		if(keyCode == EventKeyboard::KeyCode::KEY_ESCAPE)
		{
			gameEnd();
			return;
		}

		if(mKeyMap.find(keyCode) != mKeyMap.end())
		{
			Key::MappedKey key = mKeyMap[keyCode];
			if(mKeyState[key] == Key::KEY_UP)
			{
				mKeyState[key] = Key::KEY_DOWN;

				// TODO calculate position
				Event::KeyEvent event(key, Key::KEY_DOWN, 0);
				Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Event::KeyEvent::DOWN, &event);
			}

			if(key == Key::SpeedDown)
			{
				setSpeed(mSpeed - 0.5f);
			}
			else if(key == Key::SpeedUp)
			{
				setSpeed(mSpeed + 0.5f);
			}
		}
	}


	void GameScene::onKeyUp(EventKeyboard::KeyCode keyCode, cocos2d::Event* e)
	{
		if(mKeyMap.find(keyCode) != mKeyMap.end())
		{
			Key::MappedKey key = mKeyMap[keyCode];
			mKeyState[key] = Key::KEY_UP;
			Event::KeyEvent event(key, Key::KEY_UP, 0); // TODO calculate position
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Event::KeyEvent::UP, &event);
		}
	}

	void GameScene::setSpeed(float newSpeed)
	{
		if(newSpeed < 0.5f)
		{
			newSpeed = 0.5f;
		}
		else if(newSpeed > 100)
		{
			newSpeed = 100.0f;
		}

		if(newSpeed != mSpeed)
		{
			Event::SpeedEvent event(newSpeed);
			mSpeed = newSpeed;
			Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Event::SpeedEvent::CHANGED, &event);
		}
	}

	bool GameScene::loadGame()
	{
		BmsParser parser;
		bool parseResult = parser.compile(mSong->getPath().c_str(), mBmsDocument);
		if( !parseResult )
			return false;

		char* supportFormat[] = {"bmp", "jpg", "png"};
		int supportLen = sizeof(supportFormat) / sizeof(supportFormat[0]);

		// image(bga)
		for(auto it = mBmsDocument.bga().begin(); it != mBmsDocument.bga().end(); it++)
		{
			std::string path = mBmsDocument.header().getParentPath() + it->second;
			unsigned dotPos = path.find_last_of('.');
			if(dotPos != std::string::npos)
				path = path.substr(0, dotPos+1);

			for(int i=0; i<supportLen; i++)
			{
				FILE* checkExist = fopen((path + supportFormat[i]).c_str(), "rb");
				if(!checkExist)
					continue;
				fclose(checkExist);

				auto image = new Image();
				//if(false == image->initWithImageFileThreadSafe((path + supportFormat[i]).c_str(), computeImageFormatType(path + supportFormat[i])))
				if (!image->initWithImageFile((path + supportFormat[i]).c_str()))
				{
					delete image;
					continue;
				}

				mImageDictionary[it->first] = image;
				break;
			}
		}

		// wave
		for(auto it = mBmsDocument.wave().begin(); it != mBmsDocument.wave().end(); it++)
		{
			std::string fullpath = mBmsDocument.header().getParentPath() + it->second;
			mWavDictionary[it->first] = Audio::AudioManager::instance()->loadSound(fullpath.c_str());
		}

		return true;
	}

	void GameScene::onBpmChanged(cocos2d::EventCustom* e)
	{
		Event::BpmEvent* bpmEvent = static_cast<Event::BpmEvent*>(e->getUserData());
		mLastBpmMeter = bpmEvent->drawPosition();
		mLastBpmTime = bpmEvent->timestamp();
		mCurrentBpm = bpmEvent->bpm();
	}

	void GameScene::onJudge(cocos2d::EventCustom* e)
	{
		Event::JudgeEvent* judgeEvent = static_cast<Event::JudgeEvent*>(e->getUserData());
		int baseScore = 0;

		switch(judgeEvent->judge())
		{
		case JUDGE_KOOL:
			baseScore = 150;
			mCombo++;
			break;
		case JUDGE_COOL:
			baseScore = 100;
			mCombo++;
			break;
		case JUDGE_GOOD:
			baseScore = 50;
			mCombo++;
			break;

		case JUDGE_MISS2:
		case JUDGE_FAIL:
			mCombo = 0;
			break;
		}

		mScore += baseScore;

		Event::ComboEvent comboEvent(mCombo, mScore);
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Event::ComboEvent::CHANGED, &comboEvent);
	}

	void GameScene::start(float dt)
	{
		bool autoPlay = true;

		// apply loaded image
		for(auto it = mImageDictionary.begin(); it != mImageDictionary.end(); it++)
		{
			CCTexture2D* texture = new CCTexture2D();
			if(false == texture->initWithImage(it->second))
			{
				delete texture;
				continue;
			}

			mBgaDictionary[it->first] = texture;
		}

		for(auto it = mBmsDocument.channel().begin(); it != mBmsDocument.channel().end(); it++)
		{
			const std::string& channel = it->first;
			if(channel.find("01") != std::string::npos)
			{
				mChannelPlayers[channel] = new WaveChannelPlayer(it->second, mWavDictionary);
			}
			else if(channel.find("02") != std::string::npos)
			{
				//mChannelPlayers[channel] = new BarScaleChannelPlayer(it->second);
			}
			else if(channel.find("03") != std::string::npos)
			{
				mChannelPlayers[channel] = new BpmChannelPlayer(it->second);
			}
			else 
			{
				int iChannel = base36ToInt(channel.c_str());
				if(base36ToInt("10") < iChannel && iChannel < base36ToInt("70") && channel[1] != '0'/*exclude 20, 30, 40, ...*/)
				{
					int mappedKey = -1;
					for(int i=0; i<sizeof(keyChannelMap)/sizeof(keyChannelMap[0]); i++)
					{
						if(channel == keyChannelMap[i])
						{
							mappedKey = i;
							break;
						}
					}
					if(mappedKey < 0)
					{
						mChannelPlayers[channel] = new WaveChannelPlayer(it->second, mWavDictionary);
					}
					else
					{
						mChannelPlayers[channel] = new PlayChannelPlayer(it->second, mWavDictionary, Key::MappedKey(mappedKey), autoPlay);
					}
				}
				else if(
					base36ToInt(ChannelType::BGA_BASE) == iChannel ||
					base36ToInt(ChannelType::BGA_LAYER) == iChannel ||
					base36ToInt(ChannelType::BGA_LAYER2) == iChannel
					)
				{
					BgaChannelPlayer* bgaPlayer = new BgaChannelPlayer(it->second, mBgaDictionary, base36ToInt(ChannelType::BGA_POOR) == iChannel);
					mChannelPlayers[channel] = bgaPlayer;
					bgaPlayer->getSprite()->setContentSize(CCSizeMake(640, 480));
					bgaPlayer->getSprite()->setPosition(ccp(640, 480));
					addChild(bgaPlayer->getSprite());
				}
			}
		}

		{//panel
			// load skin
			CCTexture2D* skinTexture = CCTextureCache::sharedTextureCache()->addImage(Panel::SKIN_PATH);
			// no antialias
			skinTexture->setAliasTexParameters();

			CCNode* panel = CCNode::create();
			addChild(panel);

			Panel* frontPanel = new FrontPanel;
			Panel* backPanel = new BackPanel(mChannelPlayers);
			Panel* controlPanel = new ControlPanel;

			panel->addChild(backPanel);
			panel->addChild(frontPanel);
			panel->addChild(controlPanel);

			// fit panel to screen
			panel->setScale(CCDirector::sharedDirector()->getOpenGLView()->getDesignResolutionSize().height / 480);
		}

		mCurrentBpm = mBmsDocument.header().getBpm();

		//끝낼 마디 설정
		mEndPosition = 0;
		for(auto it = mBmsDocument.channel().begin(); it != mBmsDocument.channel().end(); it++)
		{
			if(it->second->endPosition() > mEndPosition)
				mEndPosition = it->second->endPosition();
		}
		mEndPosition += 1;

		// 시간 관련 초기화
		mLastBpmTime = mStartTime = currentTime();
		mLastBpmMeter = 0;
		origTime = mStartTime;// - 60000;
		origMadi = origPosit = 0;
		mBarScale = 1.0f;
		nowMadi = -1;

		// TODO 플레이노트 초기화(시간, 키 수 등)

		bMiss = false;
		bAuto = true;
		mScore = 0;
		mCombo = 0;

		// emit reset
		Event::GameConfigEvent configEvent(*mSong, 1, autoPlay, mStartTime);
		Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(Event::GameConfigEvent::RESET, &configEvent);
	}

}