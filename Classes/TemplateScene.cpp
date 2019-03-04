#include "TemplateScene.h"
#include "SimpleAudioEngine.h"
#include "CGameManager.h"

USING_NS_CC;

TemplateScene::TemplateScene() {
	cocos2d::log("TemplateScene :: ctr...");

	// produce true random numbers?!
	//time_t theTime;
	srand(time(NULL));
	//srand(time(&theTime));

	//int i;
	//for (i = 0; i < 100; i++) {
	//	cocos2d::log("rand : i = %d : %d", i, rand()%10);
	//}

	m_seq = 0.0f;
	m_cyc = 0;
}

TemplateScene::~TemplateScene() {
	cocos2d::log("TemplateScene :: dtr...");
}

Scene* TemplateScene::createScene()
{
	return TemplateScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in TemplateSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool TemplateScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	//Director::getInstance()->setClearColor(Color4F(0.0f, 1.0f, 0.0f, 0.0f));
	// Adds a colour layer to background (default black)
	//LayerColor *_bgColor = LayerColor::create(Color4B(60, 191, 240, 255));
	//this->addChild(_bgColor, -10);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Register Touch Event
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	pListener = EventListenerTouchOneByOne::create();

	pListener->onTouchBegan = CC_CALLBACK_2(TemplateScene::onTouchBegan, this);
	pListener->onTouchMoved = CC_CALLBACK_2(TemplateScene::onTouchMoved, this);
	pListener->onTouchEnded = CC_CALLBACK_2(TemplateScene::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"button-close64x64.png",
		"button-close-select64x64.png",
		CC_CALLBACK_1(TemplateScene::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'button-close64x64.png' and 'button-close-select64x64.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2 - 5;
		float y = origin.y + visibleSize.height - closeItem->getContentSize().height / 2 - 5 - 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// add a "play" icon to replay. it's an autorelease object
	auto playItem = MenuItemImage::create(
		"button-play64x64.png",
		"button-play-select64x64.png",
		CC_CALLBACK_1(TemplateScene::menuPlayCallback, this));

	if (playItem == nullptr ||
		playItem->getContentSize().width <= 0 ||
		playItem->getContentSize().height <= 0)
	{
		problemLoading("'button-play64x64.png' and 'button-play-select64x64.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - (playItem->getContentSize().width / 2) * 3.5f - 5 - 2;
		float y = origin.y + visibleSize.height - playItem->getContentSize().height / 2 - 5 - 2;
		playItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, playItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 255);

	/////////////////////////////
	// 3. add your codes below...

	// add a label shows "Hello World"
	// create and initialize a label

	auto label = Label::createWithTTF("TemplateScene v.1.0.1", "fonts/arial.ttf", 18);
	if (label == nullptr) {
		problemLoading("'fonts/arial.ttf'");
	}
	else {
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + visibleSize.width * 0.5f,
			origin.y + visibleSize.height - label->getContentSize().height));

		label->setTextColor(Color4B(255, 255, 255, 255));
		// add the label as a child to this layer
		this->addChild(label, 2);
	}

	// Add Sprite
	m_pSprite = Sprite::create("face64x64.png");
	m_pSprite->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	m_pSprite->setRotation(45.0f);
	addChild(m_pSprite);

	// start update loop
	scheduleUpdate();

	return true;
}

void TemplateScene::update(float dt)
{
	//cocos2d::log("update...");

	m_seq++;
	m_cyc++;
}

void TemplateScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);
}

void TemplateScene::menuPlayCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	// replace scene
	CGameManager::Instance()->m_stringGameType = "Game";
	CGameManager::Instance()->RunScene(kViewMapTap);
}

bool TemplateScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("TemplateScene::onTouchBegan : You touched %f, %f", location.x, location.y);

	return true;
}

void TemplateScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
}

void TemplateScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("TemplateScene::onTouchEnded : You touched %f, %f", location.x, location.y);
}