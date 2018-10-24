#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
//Test
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
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 255);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("How are you?", "fonts/arial.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

	auto labelMsg = Label::createWithTTF("nincs mas        szia          nem   tudom       ehes vagyok    hogy  vagy", "fonts/arial.ttf", 20);
	labelMsg->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + visibleSize.height * 0.5f - labelMsg->getContentSize().height + 200.0f));
	this->addChild(labelMsg, 1);

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
	float sx = (visibleSize.width * 1.0f / 8.0f) * 1.1f;
	float sy = 2.0f * 1.0f / 8.0f;
	auto mySpriteGround = Sprite::create("white8x8.png");
	if (mySpriteGround != nullptr) {
		mySpriteGround->setOpacity(255);
		mySpriteGround->setScaleX(sx);
		mySpriteGround->setScaleY(sy);
		mySpriteGround->setPosition(Vec2(visibleSize.width * 0.5f,
										 mySpriteGround->getContentSize().height* 0.5f + 50.0f));
		this->addChild(mySpriteGround, 1);
	}

	auto mySpriteGround2 = Sprite::create("white8x8.png");
	if (mySpriteGround2 != nullptr) {
		mySpriteGround2->setOpacity(255);
		mySpriteGround2->setColor(Color3B(100, 255, 100));
		mySpriteGround2->setScaleX(sx);
		float s = 52.0f * (1.0f / 8.0f);
		mySpriteGround2->setScaleY(s);
		mySpriteGround2->setPosition(Vec2(visibleSize.width * 0.5f,
										  mySpriteGround2->getContentSize().height* 0.5f * s));
		this->addChild(mySpriteGround2, 1);
	}

	// player
	m_pSpritePlayer = Sprite::create("face64x64.png");
	if (m_pSpritePlayer != nullptr) {
		float f = 0.5f; // 50% percent reduction in size
		m_pSpritePlayer->setOpacity(255);
		m_pSpritePlayer->setRotation(0);
		m_pSpritePlayer->setScale(f);
		m_pSpritePlayer->setPosition(Vec2(m_pSpritePlayer->getContentSize().width*0.5f*f + 40.0f,
								   m_pSpritePlayer->getContentSize().height*0.5f*f + mySpriteGround->getContentSize().height*0.5f + 50.0f));
		m_pSpritePlayer->setVisible(true);

		// glowing
		//auto tintTo = TintTo::create(1.0f, Color3B(255, 255, 255));
		//auto tintTo2 = TintTo::create(0.2f, Color3B(255, 0, 0));
		//seqTint = Sequence::create(tintTo, tintTo2, nullptr);
		//m_pSpritePlayer->runAction(RepeatForever::create(seqTint));
		//m_pSpritePlayer->runAction(seqTint);

		// shaking
		//auto moveTo = MoveBy::create(0.05f, Vec2(1.0f, 0.0f));
		//m_pSpritePlayer->runAction(moveTo);
		//auto moveTo2 = MoveBy::create(0.05f, Vec2(-1.0f*2, 0.0f));
		//auto moveTo3 = MoveBy::create(0.05f, Vec2(1.0f*2, 0.0f));
		//seqShake = Sequence::create(moveTo2, moveTo3, nullptr);
		//m_pSpritePlayer->runAction(RepeatForever::create(seqShake));
		this->addChild(m_pSpritePlayer, 1);
	}

	// vertical marker line guide (lazer beam)
	m_pSpriteLazer = Sprite::create("white8x8.png");
	if (m_pSpriteLazer != nullptr) {
		m_pSpriteLazer->setOpacity(255);//0
		m_pSpriteLazer->setScaleX(1.0f * 1.0f / 8.0f);
		m_pSpriteLazer->setScaleY(0.0f * 1.0f / 8.0f);
		m_pSpriteLazer->setAnchorPoint(Vec2(0.5f, 0.0f));
		m_pSpriteLazer->setPosition(Vec2(m_pSpritePlayer->getPosition().x,
										visibleSize.height * 0.0f + m_pSpritePlayer->getContentSize().height*0.5f + 50.0f));

		//// glowing lazer beam fx
		//auto fadeTo = FadeTo::create(1.0f, 0.0f);
		//auto fadeTo2 = FadeTo::create(0.2f, 255.0f);
		//seqGlow = Sequence::create(fadeTo, fadeTo2, nullptr);
		////m_pSpriteLazer->runAction(RepeatForever::create(seqGlow));

		//// growing lazer beam fx
		//auto scaleTo = ScaleTo::create(1.0f, 1.0f * 1.0f / 8.0f, 0.0f);
		//auto scaleTo2 = ScaleTo::create(0.2f, 2.0f * 1.0f / 8.0f, visibleSize.height * 1.0f / 8.0f);
		//seqScale = Sequence::create(scaleTo, scaleTo2, nullptr);
		////m_pSpriteLazer->runAction(RepeatForever::create(seqScale));

		this->addChild(m_pSpriteLazer, 0);
	}

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

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("HelloWorld::onTouchBegan : You touched %f, %f", location.x, location.y);
	if (m_pSpritePlayer) {
		// glowing
		//auto tintTo = TintTo::create(1.0f, Color3B(255, 255, 255));
		auto tintTo2 = TintTo::create(0.2f, Color3B(255, 0, 0));
		//seqTint = Sequence::create(RepeatForever::create(seqShake), tintTo, tintTo2, nullptr);
		m_pSpritePlayer->runAction(tintTo2);
	}
	if (m_pSpriteLazer) {
		//cocos2d::log("m_pSpriteLazer->getScaleY() : %f", m_pSpriteLazer->getScaleY());
		//cocos2d::log("(visibleSize.height * (1.0f / 8.0f) *  0.5f) : %f", (visibleSize.height * (1.0f / 8.0f) *  0.5f));
		//m_pSpriteLazer->stopAllActions();
		//m_pSpriteLazer->runAction(RepeatForever::create(seqGlow));
		//m_pSpriteLazer->runAction(RepeatForever::create(seqScale));
		
		// glowing lazer beam fx
		//auto fadeTo = FadeTo::create(1.0f, 0.0f);
		auto fadeTo2 = FadeTo::create(0.2f, 255.0f);
		//seqGlow = Sequence::create(fadeTo, fadeTo2, nullptr);
		m_pSpriteLazer->runAction(fadeTo2);
		
		// growing lazer beam fx
		//auto scaleTo = ScaleTo::create(1.0f, 1.0f * 1.0f / 8.0f, 0.0f);
		auto scaleTo2 = ScaleTo::create(0.2f, 2.0f * 1.0f / 8.0f, visibleSize.height * 1.0f / 8.0f);
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
		// glowing
		m_pSpritePlayer->stopAllActions();
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
		
		// growing lazer beam fx
		//auto scaleTo = ScaleTo::create(1.0f, 1.0f * 1.0f / 8.0f, 0.0f);
		//auto scaleTo2 = ScaleTo::create(0.2f, 2.0f * 1.0f / 8.0f, visibleSize.height * 1.0f / 8.0f);
		//seqScale = Sequence::create(scaleTo, scaleTo2, nullptr);
		//m_pSpriteLazer->runAction(scaleTo);
	}
}
