#include "Panel.h"
#include "Combo.h"
#include "enums.h"
#include "component/PlayNoteRenderer.h"
#include "event/JudgeEvent.h"
#include "event/GameConfigEvent.h"
#include "event/KeyEvent.h"
#include "event/SpeedEvent.h"
#include "scene/GameScene.h"
using namespace cocos2d;

namespace BMS
{
	const char* Panel::SKIN_PATH = "skin/rdmPlay.bmp";

	Panel::Panel()
	{
		Node::autorelease();

		auto dispatcher = Director::getInstance()->getEventDispatcher();
		dispatcher->addCustomEventListener(Event::GameConfigEvent::RESET, CC_CALLBACK_1(Panel::onReset, this));
		dispatcher->addCustomEventListener(Event::JudgeEvent::JUDGE, CC_CALLBACK_1(Panel::onJudge, this));
		dispatcher->addCustomEventListener(Event::SpeedEvent::CHANGED, CC_CALLBACK_1(Panel::onSpeedChange, this));
		dispatcher->addCustomEventListener(Event::KeyEvent::DOWN, CC_CALLBACK_1(Panel::onKeyDown, this));
		dispatcher->addCustomEventListener(Event::KeyEvent::UP, CC_CALLBACK_1(Panel::onKeyUp, this));
	}


	BackPanel::BackPanel(const std::map<std::string, ChannelPlayer*>& players) :
		mChannelPlayers(players)
	{
	}

	void BackPanel::onJudge(EventCustom* event)
	{
		Event::JudgeEvent* judgeEvent = static_cast<Event::JudgeEvent*>(event->getUserData());
		switch(judgeEvent->judge())
		{
		case JUDGE_MISS1:
		case JUDGE_MISS2:
		case JUDGE_FAIL:
			break;
		}
	}

	void BackPanel::onKeyDown(EventCustom* event)
	{
		Event::KeyEvent* keyEvent = static_cast<Event::KeyEvent*>(event->getUserData());
		auto pair = mLightSprites.find(keyEvent->keyCode());
		if(pair != mLightSprites.end())
		{
			pair->second->setVisible(true);
		}
	}

	void BackPanel::onKeyUp(EventCustom* event)
	{
		Event::KeyEvent* keyEvent = static_cast<Event::KeyEvent*>(event->getUserData());
		auto pair = mLightSprites.find(keyEvent->keyCode());
		if(pair != mLightSprites.end())
		{
			pair->second->setVisible(false);
		}
	}

	static Sprite* addHorizontal(SpriteBatchNode* parent, const Rect& area, bool initLeft = false)
	{
		static Sprite* leftObject = NULL;
		if(initLeft) {
			leftObject = NULL;
		}

		const Point& position = leftObject == NULL ?
										Point::ZERO :
										Point(leftObject->getPosition().x + leftObject->getContentSize().width, leftObject->getPosition().y);
		Sprite* sprite = Sprite::createWithTexture(parent->getTexture(), area);
		sprite->setPosition(position);
		sprite->setAnchorPoint(Point::ZERO);
		parent->addChild(sprite);

		leftObject = sprite;
		return sprite;
	}

	static Sprite* addOverlay(SpriteBatchNode* parent, const Rect& area, Sprite* original)
	{
		Sprite* sprite = Sprite::createWithTexture(parent->getTexture(), area);
		sprite->setPosition(original->getPosition());
		sprite->setAnchorPoint(Point::ZERO);
		sprite->setVisible(false);
		parent->addChild(sprite);
		return sprite;
	}

	void BackPanel::onReset(EventCustom* event)
	{
		Event::GameConfigEvent* configEvent = static_cast<Event::GameConfigEvent*>(event->getUserData());
		removeAllChildren();

		// base
		Texture2D* skinTexture = TextureCache::sharedTextureCache()->addImage(SKIN_PATH);
		SpriteBatchNode* batchNode = SpriteBatchNode::createWithTexture(skinTexture);
		addChild(batchNode);

		Rect rtScratch = Rect(3, 1, 30, 420);
		Rect rtChannel1 = Rect(35, 1, 18, 420);
		Rect rtChannel2 = Rect(54, 1, 18, 420);
		Rect rtScratchLine = Rect(33, 1, 2, 420);
		Rect rtChannel1Line = Rect(53, 1, 1, 420);
		Rect rtChannel2Line = Rect(72, 1, 1, 420);
		Rect rtLeftAccesory = Rect(435, 35, 25, 420);
		Rect rtRightAccesory = Rect(460, 35, 12, 420);
		Rect rtLeftBottom = Rect(216, 1, 25, 45);
		Rect rtRightBottom = Rect(241, 1, 12, 45);

		Sprite* leftBottom = addHorizontal(batchNode, rtLeftBottom, true);

		Sprite* leftAccesory = addHorizontal(batchNode, rtLeftAccesory, true);
		leftAccesory->setPosition(Point(0, leftBottom->getContentSize().height));
		Sprite* scratch = addHorizontal(batchNode, rtScratch);
		addHorizontal(batchNode, rtScratchLine);
		Sprite* key1 = addHorizontal(batchNode, rtChannel1);
		addHorizontal(batchNode, rtChannel1Line);
		Sprite* key2 = addHorizontal(batchNode, rtChannel2);
		addHorizontal(batchNode, rtChannel2Line);
		Sprite* key3 = addHorizontal(batchNode, rtChannel1);
		addHorizontal(batchNode, rtChannel1Line);
		Sprite* key4 = addHorizontal(batchNode, rtChannel2);
		addHorizontal(batchNode, rtChannel2Line);
		Sprite* key5 = addHorizontal(batchNode, rtChannel1);
		Sprite* rightAccesory = addHorizontal(batchNode, rtRightAccesory);

		Sprite* rightBottom = addHorizontal(batchNode, rtRightBottom);
		rightBottom->setPosition(Point(rightAccesory->getPositionX(), leftBottom->getPositionY()));


		// overlay
		ccBlendFunc overlayFunc = {GL_ONE, GL_ONE};
		SpriteBatchNode* overlayBatchNode = SpriteBatchNode::createWithTexture(skinTexture);
		overlayBatchNode->setBlendFunc(overlayFunc);
		addChild(overlayBatchNode);

		Rect rtScratchLight = Rect(73, 1, 30, 420);
		Rect rtChannel1Light = Rect(105, 1, 18, 420);
		Rect rtChannel2Light = Rect(124, 1, 18, 420);

		mLightSprites[Key::Player1_Scratch] = addOverlay(overlayBatchNode, rtScratchLight, scratch);
		mLightSprites[Key::Player1_1] = addOverlay(overlayBatchNode, rtChannel1Light, key1);
		mLightSprites[Key::Player1_2] = addOverlay(overlayBatchNode, rtChannel2Light, key2);
		mLightSprites[Key::Player1_3] = addOverlay(overlayBatchNode, rtChannel1Light, key3);
		mLightSprites[Key::Player1_4] = addOverlay(overlayBatchNode, rtChannel2Light, key4);
		mLightSprites[Key::Player1_5] = addOverlay(overlayBatchNode, rtChannel1Light, key5);


		// note
		std::map<std::string, ChannelPlayer*>& ncPlayers = const_cast<std::map<std::string, ChannelPlayer*>&>(mChannelPlayers);

		Rect rtScratchNote = Rect(143, 93, 30, 21);
		Rect rtChannel1Note = Rect(174, 93, 18, 21);
		Rect rtChannel2Note = Rect(193, 93, 18, 21);

		Sprite* scratchNote = Sprite::createWithTexture(skinTexture, rtScratchNote);
		Sprite* channel1Note = Sprite::createWithTexture(skinTexture, rtChannel1Note);
		Sprite* channel2Note = Sprite::createWithTexture(skinTexture, rtChannel2Note);

		PlayNoteRenderer* scratchRenderer = new PlayNoteRenderer((PlayChannelPlayer*)ncPlayers["16"], scratchNote);
		PlayNoteRenderer* key1Renderer = new PlayNoteRenderer((PlayChannelPlayer*)ncPlayers["11"], channel1Note);
		PlayNoteRenderer* key2Renderer = new PlayNoteRenderer((PlayChannelPlayer*)ncPlayers["12"], channel2Note);
		PlayNoteRenderer* key3Renderer = new PlayNoteRenderer((PlayChannelPlayer*)ncPlayers["13"], channel1Note);
		PlayNoteRenderer* key4Renderer = new PlayNoteRenderer((PlayChannelPlayer*)ncPlayers["14"], channel2Note);
		PlayNoteRenderer* key5Renderer = new PlayNoteRenderer((PlayChannelPlayer*)ncPlayers["15"], channel1Note);

		scratchRenderer->setPosition(mLightSprites[Key::Player1_Scratch]->getPosition());
		key1Renderer->setPosition(mLightSprites[Key::Player1_1]->getPosition());
		key2Renderer->setPosition(mLightSprites[Key::Player1_2]->getPosition());
		key3Renderer->setPosition(mLightSprites[Key::Player1_3]->getPosition());
		key4Renderer->setPosition(mLightSprites[Key::Player1_4]->getPosition());
		key5Renderer->setPosition(mLightSprites[Key::Player1_5]->getPosition());

		scratchRenderer->reset(configEvent);
		key1Renderer->reset(configEvent);
		key2Renderer->reset(configEvent);
		key3Renderer->reset(configEvent);
		key4Renderer->reset(configEvent);
		key5Renderer->reset(configEvent);

		addChild(scratchRenderer);
		addChild(key1Renderer);
		addChild(key2Renderer);
		addChild(key3Renderer);
		addChild(key4Renderer);
		addChild(key5Renderer);
	}



	FrontPanel::FrontPanel() {}

	void FrontPanel::onReset(EventCustom* event)
	{
		removeAllChildren();

		// base
		SpriteBatchNode* batchNode = SpriteBatchNode::create(SKIN_PATH);
		addChild(batchNode);

		Rect rtScratch = Rect(143, 1, 34, 45);
		Rect rtChannel1 = Rect(177, 1, 19, 45);
		Rect rtChannel2 = Rect(196, 1, 19, 45);

		Sprite* scratch = addHorizontal(batchNode, rtScratch, true);
		scratch->setPosition(Point(24, 0));	// 25 is rtLeftBottom's width
		Sprite* key1 = addHorizontal(batchNode, rtChannel1);
		Sprite* key2 = addHorizontal(batchNode, rtChannel2);
		Sprite* key3 = addHorizontal(batchNode, rtChannel1);
		Sprite* key4 = addHorizontal(batchNode, rtChannel2);
		Sprite* key5 = addHorizontal(batchNode, rtChannel1);


		// overlay
		SpriteBatchNode* overlayBatchNode = SpriteBatchNode::create(SKIN_PATH);
		addChild(overlayBatchNode);

		Rect rtScratchPress = Rect(143, 47, 34, 45);
		Rect rtChannel1Press = Rect(177, 47, 19, 45);
		Rect rtChannel2Press = Rect(196, 47, 19, 45);

		mButtonPress[Key::Player1_Scratch] = addOverlay(overlayBatchNode, rtScratchPress, scratch);
		mButtonPress[Key::Player1_1] = addOverlay(overlayBatchNode, rtChannel1Press, key1);
		mButtonPress[Key::Player1_2] = addOverlay(overlayBatchNode, rtChannel2Press, key2);
		mButtonPress[Key::Player1_3] = addOverlay(overlayBatchNode, rtChannel1Press, key3);
		mButtonPress[Key::Player1_4] = addOverlay(overlayBatchNode, rtChannel2Press, key4);
		mButtonPress[Key::Player1_5] = addOverlay(overlayBatchNode, rtChannel1Press, key5);

		Node* combo = new Combo();
		addChild(combo);
		combo->setPosition(Point(key3->getPositionX(), key3->getPositionY() + 130));
	}

	void FrontPanel::onKeyDown(EventCustom* event)
	{
		Event::KeyEvent* keyEvent = static_cast<Event::KeyEvent*>(event->getUserData());
		auto pair = mButtonPress.find(keyEvent->keyCode());
		if(pair != mButtonPress.end())
		{
			pair->second->setVisible(true);
		}
	}

	void FrontPanel::onKeyUp(EventCustom* event)
	{
		Event::KeyEvent* keyEvent = static_cast<Event::KeyEvent*>(event->getUserData());
		auto pair = mButtonPress.find(keyEvent->keyCode());
		if(pair != mButtonPress.end())
		{
			pair->second->setVisible(false);
		}
	}



	ControlPanel::ControlPanel() {}

	void ControlPanel::onSpeedChange(EventCustom* event)
	{

	}

	void ControlPanel::onReset(EventCustom* event)
	{

	}
}