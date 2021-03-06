#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "CGameManager.h"

USING_NS_CC;

HelloWorld::HelloWorld() {
	cocos2d::log("ctr...");

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

HelloWorld::~HelloWorld() {
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

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	//Director::getInstance()->setClearColor(Color4F(0.0f, 1.0f, 0.0f, 0.0f));
	//LayerColor *_bgColor = LayerColor::create(Color4B(204, 230, 255, 255));
	/*LayerColor *_bgColor = LayerColor::create(Color4B(51, 119, 255, 255));
	this->addChild(_bgColor, -10);*/
	LayerGradient *_bgColor2 = LayerGradient::create(Color4B(51, 119, 255, 255), Color4B(0, 60, 179, 255), Vec2(0.0f, 1.0f));
	this->addChild(_bgColor2, -10);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//////////////////////////////////////////////////////////////////////////////////////////////
	// Register Touch Event
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	pListener = EventListenerTouchOneByOne::create();

	pListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	pListener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	pListener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(pListener, this);


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "close.png",
                                           "close_select.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

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
		CC_CALLBACK_1(HelloWorld::menuPlayCallback, this));

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

	//auto labelMsg = Label::createWithTTF("nincs mas        szia          nem   tudom       ehes vagyok    hogy  vagy", "fonts/arial.ttf", 20);
	//labelMsg->setPosition(Vec2(origin.x + visibleSize.width / 2,
	//	origin.y + visibleSize.height * 0.5f - labelMsg->getContentSize().height + 200.0f));
	//this->addChild(labelMsg, 1);

 //   // add "HelloWorld" splash screen"
 //   auto sprite = Sprite::create("HelloWorld.png");
 //   if (sprite == nullptr)
	//{
	//	problemLoading("'HelloWorld.png'");
	//}
 //   else
 //   {
 //       // position the sprite on the center of the screen
 //       sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

 //       // add the sprite as a child to this layer
 //       this->addChild(sprite, 0);
 //   }

	// horizontal floor
	//float sx = (visibleSize.width * 1.0f / 8.0f) * 1.1f;
	//float sy = 2.0f * 1.0f / 8.0f;
	//auto mySpriteGround = Sprite::create("white8x8.png");
	//if (mySpriteGround != nullptr) {
	//	mySpriteGround->setOpacity(255);
	//	mySpriteGround->setScaleX(sx);
	//	mySpriteGround->setScaleY(sy);
	//	mySpriteGround->setPosition(Vec2(visibleSize.width * 0.5f,
	//									 mySpriteGround->getContentSize().height* 0.5f + 50.0f));
	//	this->addChild(mySpriteGround, 1);
	//}

	//auto mySpriteGround2 = Sprite::create("white8x8.png");
	//if (mySpriteGround2 != nullptr) {
	//	mySpriteGround2->setOpacity(255);
	//	mySpriteGround2->setColor(Color3B(100, 255, 100));
	//	mySpriteGround2->setScaleX(sx);
	//	float s = 52.0f * (1.0f / 8.0f);
	//	mySpriteGround2->setScaleY(s);
	//	mySpriteGround2->setPosition(Vec2(visibleSize.width * 0.5f,
	//									  mySpriteGround2->getContentSize().height* 0.5f * s));
	//	this->addChild(mySpriteGround2, 1);
	//}

	// player
	//m_pSpritePlayer = Sprite::create("face64x64.png");
	//if (m_pSpritePlayer != nullptr) {
	//	float f = 0.5f; // 50% percent reduction in size
	//	m_pSpritePlayer->setOpacity(255);
	//	m_pSpritePlayer->setRotation(0);
	//	m_pSpritePlayer->setScale(f);
	//	m_pSpritePlayer->setPosition(Vec2(m_pSpritePlayer->getContentSize().width*0.5f*f + 40.0f,
	//							   m_pSpritePlayer->getContentSize().height*0.5f*f + mySpriteGround->getContentSize().height*0.5f + 50.0f));
	//	m_pSpritePlayer->setVisible(true);
	//	
	//	this->addChild(m_pSpritePlayer, 1);
	//}

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
	
	bShake = false;

	pEntityManager->init();

	float globalOffsetY = -20.0f;

	// entity
	m_pSpritePlayer = pEntity->pSprite = Sprite::create("birdywalk1super64x64.png");
	float groundLevelOffsetY = -8;
	float groundLevelOffsetX = -300;
	m_pSpritePlayer->setPosition(Vec2(visibleSize.width*0.5f + groundLevelOffsetX, 64.0f + 32.0f + groundLevelOffsetY + globalOffsetY));
	this->addChild(m_pSpritePlayer, 21);

	float sx = 1.0f * 1.0f / 8.0f;
	float sy = (visibleSize.height * 1.0f / 8.0f);
	float markerOffsetX = 8.0f;
	float markerOffsetY = 32.0f - 4.0f + 24.0f;

	// vertical marker line guide (lazer beam)
	m_pSpriteLazer = Sprite::create("white8x8.png");
	if (m_pSpriteLazer != nullptr) {
		m_pSpriteLazer->setColor(Color3B(255, 0, 0));
		m_pSpriteLazer->setOpacity(192);
		m_pSpriteLazer->setScaleX(1.0f * 1.0f / 8.0f);
		m_pSpriteLazer->setScaleY(0.0f * 1.0f / 8.0f);
		m_pSpriteLazer->setAnchorPoint(Vec2(0.5f, 0.0f));
		Vec2 P = m_pSpritePlayer->getPosition();
		m_pSpriteLazer->setPosition(Vec2(P.x + markerOffsetX, P.y + visibleSize.height*0.0f + markerOffsetY + globalOffsetY));
		this->addChild(m_pSpriteLazer, 30);
		Letter::T = Vec2(P.x + markerOffsetX, -1.0f);
	}

	// vertical marker
	m_pSpriteMarker = Sprite::create("white8x8.png");
	if (m_pSpriteMarker != nullptr) {
		m_pSpriteMarker->setColor(Color3B(0, 0, 0));
		m_pSpriteMarker->setOpacity(96);
		m_pSpriteMarker->setScaleX(sx);
		m_pSpriteMarker->setScaleY(sy);
		Vec2 P = m_pSpritePlayer->getPosition();
		m_pSpriteMarker->setPosition(Vec2(P.x + markerOffsetX, P.y + visibleSize.height*0.5f + markerOffsetY + globalOffsetY));
		this->addChild(m_pSpriteMarker, 29);
	}

	animFrames.reserve(2);
	animFrames.pushBack(SpriteFrame::create("birdywalk1super64x64.png", Rect(0, 0, 64, 64)));
	animFrames.pushBack(SpriteFrame::create("birdywalk2super64x64.png", Rect(0, 0, 64, 64)));

	// create the animation out of the frames
	animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	animate = Animate::create(animation);

	// run it and repeat it forever
	m_pSpritePlayer->runAction(RepeatForever::create(animate));

	// player rocking for side-to-side
	rotateToLeft = RotateTo::create(0.07f, 6.0f);
	rotateToRight = RotateTo::create(0.07f, -6.0f);
	seqJelly = Sequence::create(rotateToLeft, rotateToRight, nullptr);
	repeatSeqJelly = RepeatForever::create(seqJelly);
	m_pSpritePlayer->runAction(repeatSeqJelly);
	repeatSeqJelly->retain();
	seqJelly->retain();
	rotateToLeft->retain();
	rotateToRight->retain();
	
	// letter
	/*pLetter->spawn("face64x64.png", Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	pEntityManager->addEntity(pLetter);*/

	int n = 0;
	Letter* letter = nullptr;
	
	/*for (n = 0; n < 3; n++) {
		letter = new Letter(this);
		letter->spawn("B", 1, "white8x8.png", Vec2(200.0f + n * 320.0f, visibleSize.height*0.5f + n * 60));
		pEntityManager->addEntity(letter);
	}*/

	std::string french = "Bon  Bon  jour  bononoj  Bon  on  Jour  Jourouru  Bonjour";
	std::string s;// = std::string(1, french[3]);
	int length = french.size();
	float letterWidth = 16.0f;
	//cocos2d::log("french[3]: %c", french[3]);
	//cocos2d::log("s: %s", s.c_str());
	int m = -1;
	int i;
	for (i = 0; i < 3; i++) {
		for (n = 0; n <length; n++) {
			s = std::string(1, french[n]);
			if (s != " ") { // ignore white-space characters
				letter = new Letter(this);
				letter->spawn(s, i + 2, "white8x8.png", Vec2(200.0f + m * 320.0f + n*(letterWidth+4), visibleSize.height*0.75f - i * 60));
				pEntityManager->addEntity(letter);
			}
		}
	}
	
	Floor* floor = nullptr;
	float width = visibleSize.width;
	float tileWidth = 64.0f;
	int floorTilesN = int(width / tileWidth) + 2; // 2 extra tiles needed minimum (1 for floor of value and 1 for seamless wrapping)
	for (n = 0; n < floorTilesN; n++) {
		floor = new Floor(this);
		floor->spawn("bodygreen64x64.png", Vec2(tileWidth*0.5f + n * 64.0f, tileWidth*0.5f + globalOffsetY));
		pEntityManager->addEntity(floor);
	}


	// TODO: add factory method for creating different types of derived entities.

	// start update loop
	scheduleUpdate();

    return true;
}

void HelloWorld::update(float dt)
{
	//cocos2d::log("update...");

	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	if (m_pSpriteLazer->getScaleY() >= (visibleSize.height * (1.0f / 8.0f) *  0.75f)) // 90% of full  size
		bShake = true;
	
	shakeTheWorld(bShake);

	pEntityManager->process();
}

void HelloWorld::shakeTheWorld(bool bStatus) {
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

void HelloWorld::menuCloseCallback(Ref* pSender)
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

void HelloWorld::menuPlayCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	// replace scene
	CGameManager::Instance()->m_stringGameType = "Game";
	CGameManager::Instance()->RunScene(kViewGame);
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("HelloWorld::onTouchBegan : You touched %f, %f", location.x, location.y);

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

	return true;
}

void HelloWorld::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
}

void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("HelloWorld::onTouchEnded : You touched %f, %f", location.x, location.y);

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
}
