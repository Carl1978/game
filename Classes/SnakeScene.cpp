#include "SnakeScene.h"
#include "SimpleAudioEngine.h"
#include "CGameManager.h"

USING_NS_CC;

SnakeScene::SnakeScene() {
	cocos2d::log("SnakeScene :: ctr...");

	//std::shared_ptr<EntityManager> pEntityManager = std::make_shared<EntityManager>(this);
	//std::shared_ptr<Entity> pEntity = std::make_shared<Entity>();
	//std::shared_ptr<Letter> pLetter = std::make_shared<Letter>(this);
	
	Entity::currentID = 0;
	Floor::total = 0;
	Letter::T = Vec2(0, 0);

	pEntityManager = new EntityManager(this);
	pEntity = new Entity();
	//pLetter = new Letter(this);

	// produce true random numbers?!
	//time_t theTime;
	srand(time(NULL));
	//srand(time(&theTime));

	//int i;
	//for (i = 0; i < 100; i++) {
	//	cocos2d::log("rand : i = %d : %d", i, rand()%10);
	//}
}

SnakeScene::~SnakeScene() {
	cocos2d::log("dtr...");
	if (pEntityManager) {
		pEntityManager->deinit();
		delete pEntityManager;
		pEntityManager = nullptr;
	}

	if (pEntity) {
		if (pEntity->pSprite) {
			this->removeChild(pEntity->pSprite);
		}
		delete pEntity;
		pEntity = nullptr;
	}

	/*if (pLetter) {
		if (pLetter->pSprite) {
			this->removeChild(pLetter->pSprite);
		}
		delete pLetter;
		pLetter = nullptr;
	}*/
}

Scene* SnakeScene::createScene()
{
    return SnakeScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in SnakeSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool SnakeScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	//Director::getInstance()->setClearColor(Color4F(0.0f, 1.0f, 0.0f, 0.0f));
	//LayerColor *_bgColor = LayerColor::create(Color4B(204, 230, 255, 255));
	LayerColor *_bgColor = LayerColor::create(Color4B(80, 80, 80, 255));
	this->addChild(_bgColor, -10);
	//LayerGradient *_bgColor2 = LayerGradient::create(Color4B(51, 119, 255, 255), Color4B(0, 60, 179, 255), Vec2(0.0f, 1.0f));
	//this->addChild(_bgColor2, -10);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//////////////////////////////////////////////////////////////////////////////////////////////
	// Register Touch Event
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	pListener = EventListenerTouchOneByOne::create();

	pListener->onTouchBegan = CC_CALLBACK_2(SnakeScene::onTouchBegan, this);
	pListener->onTouchMoved = CC_CALLBACK_2(SnakeScene::onTouchMoved, this);
	pListener->onTouchEnded = CC_CALLBACK_2(SnakeScene::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(pListener, this);


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "close.png",
                                           "close_select.png",
                                           CC_CALLBACK_1(SnakeScene::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'close.png' and 'close_select.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2 - 5;
        float y = origin.y + visibleSize.height - closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

	// add a "play" icon to replay. it's an autorelease object
	auto playItem = MenuItemImage::create(
		"nextbutton32x32.png",
		"nextbutton32x32s.png",
		CC_CALLBACK_1(SnakeScene::menuPlayCallback, this));

	if (playItem == nullptr ||
		playItem->getContentSize().width <= 0 ||
		playItem->getContentSize().height <= 0)
	{
		problemLoading("'nextbutton32x32.png' and 'nextbutton32x32s.png'");
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
	/*
    // add a label shows "Hello World"
    // create and initialize a label
	
    auto label = Label::createWithTTF("1 + 1?", "fonts/arial.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

		label->setTextColor(Color4B(0, 0, 0, 255));
        // add the label as a child to this layer
        this->addChild(label, 1);
    }

	
	// air / cloud strips #1,2,3
	Color3B cloudColours[3] = {
		Color3B(0, 68, 204),
		Color3B(0, 77, 230),
		Color3B(0, 85, 255)
	};
	for (int p = 0; p < 3; p++) {
		auto spriteAir = Sprite::create("white8x8.png");
		spriteAir->setOpacity(255);
		spriteAir->setColor(cloudColours[p]);
		Vec2 airScale = Vec2(visibleSize.width * (1.0f / 8.0f), 60.0f * (1.0f / 8.0f));
		spriteAir->setScaleX(airScale.x);
		spriteAir->setScaleY(airScale.y);
		spriteAir->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height*0.75f - p * 60));
		this->addChild(spriteAir, 1);
	}
	*/
	bShake = false;

	pEntityManager->init();

	float globalOffsetY = -20.0f;
	
	float sx = 1.0f * 1.0f / 8.0f;
	float sy = (visibleSize.height * 1.0f / 8.0f);
	float markerOffsetX = 8.0f;
	float markerOffsetY = 32.0f - 4.0f + 24.0f;

	// vertical marker
	m_pSpriteMarker = Sprite::create("white8x8.png");
	if (m_pSpriteMarker != nullptr) {
		m_pSpriteMarker->setColor(Color3B(0, 0, 0));
		m_pSpriteMarker->setOpacity(96);
		m_pSpriteMarker->setScaleX(sx);
		m_pSpriteMarker->setScaleY(sy);
		m_pSpriteMarker->setPosition(Vec2(visibleSize.width*0.75f, visibleSize.height*0.5f));
		this->addChild(m_pSpriteMarker, 29);
	}

	// --------------------------------------------------------------- //
	// a simple snake 16x16 pixel boxes - 8 segments horizontally long
	//float unitScale = 1.0f / 64.0f;//1.0f / 8.0f;
	float unitScale = 1.0f / 8.0f;
	Size pixels = Size(8, 8);
	float segments = 16;
	//Vec2 scale = Vec2(pixels.width * unitScale * segments, pixels.height * 1.0f);// unitScale);
	Vec2 scale = Vec2(pixels.width * unitScale * segments, pixels.height * unitScale);

	scale.x = 1.0f;
	scale.y = 1.0f;

	// create a snake sprite
	auto spriteSnake = Sprite::create("snake8x8.png");
	//auto spriteSnake = Sprite::create("snake64x1.png");
	//auto spriteSnake = Sprite::create("snake64x1.png", Rect(0, 0, 8, 8));
	//spriteSnake->setContentSize(Size(200, 16));

	spriteSnake->setContentSize(Size(16, 8));
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	spriteSnake->getTexture()->setTexParameters(tp);
	spriteSnake->setTextureRect(Rect(0, 0, 16*8, 8));


	spriteSnake->setColor(Color3B(0, 255, 0));
	spriteSnake->setOpacity(192);
	//spriteSnake->setScaleX(scale.x);
	//spriteSnake->setScaleY(scale.y);
	spriteSnake->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	this->addChild(spriteSnake);

	// make it grow and shrink animation
	auto scaleTo = ScaleTo::create(1.0f, scale.x * 2.0f, scale.y * 2.0f);
	auto scaleTo2 = ScaleTo::create(1.0f, scale.x * 0.5f, scale.y * 0.5f);
	auto seqScale = Sequence::create(scaleTo, scaleTo2, nullptr);
	spriteSnake->runAction(RepeatForever::create(seqScale));
	// --------------------------------------------------------------- //

	// TODO: add factory method for creating different types of derived entities.

	// start update loop
	scheduleUpdate();

    return true;
}

void SnakeScene::update(float dt)
{
	//cocos2d::log("update...");

	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	//if (m_pSpriteLazer->getScaleY() >= (visibleSize.height * (1.0f / 8.0f) *  0.75f)) // 90% of full  size
	//	bShake = true;
	
	//shakeTheWorld(bShake);

	//pEntityManager->process();
}

void SnakeScene::shakeTheWorld(bool bStatus) {
	if (bStatus) {
		// shake the place!! :P
		float fx = (-25 + rand() % 51) * 0.1f;;
		float fy = (-25 + rand() % 51) * 0.1f;;
		this->setPosition(Vec2(fx, fy));
	}
	else {
		// no shaking please.
		this->setPosition(Vec2(0, 0));
	}
}

void SnakeScene::menuCloseCallback(Ref* pSender)
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

void SnakeScene::menuPlayCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	// replace scene
	CGameManager::Instance()->m_stringGameType = "Game";
	CGameManager::Instance()->RunScene(kViewGame);
}

bool SnakeScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("SnakeScene::onTouchBegan : You touched %f, %f", location.x, location.y);

	/*
	if (m_pSpritePlayer) {
		m_pSpritePlayer->stopAction(repeatSeqJelly);
		m_pSpritePlayer->stopAction(rotateToLeft);
		m_pSpritePlayer->stopAction(rotateToRight);
		rotateToShoot = RotateTo::create(0.3f, 0.0f);
		m_pSpritePlayer->runAction(rotateToShoot);

		// glowing
		//auto tintTo = TintTo::create(1.0f, Color3B(255, 255, 255));
		tint = TintTo::create(0.2f, Color3B(192, 128, 128));
		//seqTint = Sequence::create(RepeatForever::create(seqShake), tintTo, tintTo2, nullptr);
		m_pSpritePlayer->runAction(tint);
	}
	if (m_pSpriteLazer) {
		Letter::T.y = 0.0f; // indicates we are firing laser! >= 0
		//cocos2d::log("m_pSpriteLazer->getScaleY() : %f", m_pSpriteLazer->getScaleY());
		//cocos2d::log("(visibleSize.height * (1.0f / 8.0f) *  0.5f) : %f", (visibleSize.height * (1.0f / 8.0f) *  0.5f));
		//m_pSpriteLazer->stopAllActions();
		//m_pSpriteLazer->runAction(RepeatForever::create(seqGlow));
		//m_pSpriteLazer->runAction(RepeatForever::create(seqScale));
		
		// glowing lazer beam fx
		auto fadeTo = FadeTo::create(0.1f, 128.0f);
		auto fadeTo2 = FadeTo::create(0.1f, 64.0f);
		auto seqGlow = Sequence::create(fadeTo, fadeTo2, nullptr);
		m_pSpriteLazer->runAction(RepeatForever::create(seqGlow));
		//m_pSpriteLazer->runAction(fadeTo2);
		m_pSpriteMarker->setOpacity(32);
		
		// growing lazer beam fx
		//auto scaleTo = ScaleTo::create(1.0f, 1.0f * 1.0f / 8.0f, 0.0f);
		auto scaleTo2 = ScaleTo::create(0.1f, 8.0f * 1.0f / 8.0f, visibleSize.height * 1.0f / 8.0f);
		//auto scaleTo2 = ScaleTo::create(0.1f, 8.0f * 1.0f / 8.0f, visibleSize.height*0.5f * 1.0f / 8.0f);
		//seqScale = Sequence::create(scaleTo, scaleTo2, nullptr);
		m_pSpriteLazer->runAction(scaleTo2);
	}
	*/

	return true;
}

void SnakeScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
}

void SnakeScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("SnakeScene::onTouchEnded : You touched %f, %f", location.x, location.y);

	/*
	if (m_pSpritePlayer) {
		Letter::T.y = -1.0f; // indicates we are NOT firing laser! < 0
		m_pSpritePlayer->stopAction(rotateToShoot);
		rotateToLeft = RotateTo::create(0.07f, 6.0f);
		rotateToRight = RotateTo::create(0.07f, -6.0f);
		seqJelly = Sequence::create(rotateToLeft, rotateToRight, nullptr);
		repeatSeqJelly = RepeatForever::create(seqJelly);
		m_pSpritePlayer->runAction(repeatSeqJelly);

		// glowing
		//m_pSpritePlayer->stopAllActions();
		m_pSpritePlayer->stopAction(tint);
		m_pSpritePlayer->setColor(Color3B(255, 255, 255));
		//auto tintTo = TintTo::create(1.0f, Color3B(255, 255, 255));
		//auto tintTo2 = TintTo::create(0.2f, Color3B(255, 0, 0));
		//seqTint = Sequence::create(RepeatForever::create(seqShake), tintTo, tintTo2, nullptr);
		//m_pSpritePlayer->runAction(tintTo);
	}

	if (m_pSpriteLazer) {
		bShake = false;
		m_pSpriteLazer->stopAllActions();
		m_pSpriteLazer->setScaleX(1.0f * 1.0f / 8.0f);
		m_pSpriteLazer->setScaleY(0.0f * 1.0f / 8.0f);
		//m_pSpriteLazer->runAction(RepeatForever::create(seqGlow));
		//m_pSpriteLazer->runAction(RepeatForever::create(seqScale));
		
		// glowing lazer beam fx
		m_pSpriteLazer->stopAllActions();
		m_pSpriteLazer->setOpacity(0.0f);
		//auto fadeTo2 = FadeTo::create(0.2f, 255.0f);
		//seqGlow = Sequence::create(fadeTo, fadeTo2, nullptr);
		//m_pSpriteLazer->runAction(fadeTo);
		m_pSpriteMarker->setOpacity(96);
		
		// growing lazer beam fx
		//auto scaleTo = ScaleTo::create(1.0f, 1.0f * 1.0f / 8.0f, 0.0f);
		//auto scaleTo2 = ScaleTo::create(0.2f, 2.0f * 1.0f / 8.0f, visibleSize.height * 1.0f / 8.0f);
		//seqScale = Sequence::create(scaleTo, scaleTo2, nullptr);
		//m_pSpriteLazer->runAction(scaleTo);
	}
	*/
}
