#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "CGameManager.h"

// Free Assets (will add in credits)
// https://bayat.itch.io/platform-game-assets
// buttons made in GIMP: Brightness/Contrast +50
// 50x50 image resize, then canvas 64x64.

// Free licence with attribution
// https://www.freepik.com/free-vector/set-of-four-pretty-boats-of-colors_1108849.htm


USING_NS_CC;

Vec2* Letter::wavePos;

GameScene::GameScene() {
	cocos2d::log("ctr...");

	//std::shared_ptr<EntityManager> pEntityManager = std::make_shared<EntityManager>(this);
	//std::shared_ptr<Entity> pEntity = std::make_shared<Entity>();
	//std::shared_ptr<Letter> pLetter = std::make_shared<Letter>(this);
	
	Entity::currentID = 0;
	Floor::total = 0;
	Letter::T = Vec2(0, 0);

	//pEntityManager = new EntityManager(this);
	//pEntity = new Entity();
	//pLetter = new Letter(this);

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

	for (int j = 0; j < 3; j++) {
		wavePos[j] = Vec2(0.0f, 0.0f);
		for (int i = 0; i < 6; i++) {
			waveSpr[j][i] = nullptr;
		}
	}

	Letter::wavePos = &wavePos[0];
}

GameScene::~GameScene() {
	cocos2d::log("dtr...");
	/*if (pEntityManager) {
		pEntityManager->deinit();
		delete pEntityManager;
		pEntityManager = nullptr;
	}*/

	/*if (pEntity) {
		if (pEntity->pSprite) {
			this->removeChild(pEntity->pSprite);
		}
		delete pEntity;
		pEntity = nullptr;
	}*/

	/*if (pLetter) {
		if (pLetter->pSprite) {
			this->removeChild(pLetter->pSprite);
		}
		delete pLetter;
		pLetter = nullptr;
	}*/
}

Scene* GameScene::createScene()
{
    return GameScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in GameSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

	//Director::getInstance()->setClearColor(Color4F(0.0f, 1.0f, 0.0f, 0.0f));
	//LayerColor *_bgColor = LayerColor::create(Color4B(204, 230, 255, 255));
	//LayerColor *_bgColor = LayerColor::create(Color4B(0, 0, 0, 255));
	LayerColor *_bgColor = LayerColor::create(Color4B(60, 191, 240, 255));
	this->addChild(_bgColor, -10);
	/*LayerGradient *_bgColor2 = LayerGradient::create(Color4B(51, 119, 255, 255), Color4B(0, 60, 191, 240), Vec2(0.0f, 1.0f));
	this->addChild(_bgColor2, -10);*/

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();


	//////////////////////////////////////////////////////////////////////////////////////////////
	// Register Touch Event
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	pListener = EventListenerTouchOneByOne::create();

	pListener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
	pListener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
	pListener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(pListener, this);


    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "button-close64x64.png",
                                           "button-close-select64x64.png",
                                           CC_CALLBACK_1(GameScene::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'button-close64x64.png' and 'button-close-select64x64.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2 - 5;
		float y = origin.y + visibleSize.height - closeItem->getContentSize().height / 2 - 5 - 2;
        closeItem->setPosition(Vec2(x,y));
    }

	// add a "play" icon to replay. it's an autorelease object
	auto playItem = MenuItemImage::create(
		"button-play64x64.png",
		"button-play-select64x64.png",
		CC_CALLBACK_1(GameScene::menuPlayCallback, this));

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

		label->setTextColor(Color4B(255, 255, 255, 255));
        // add the label as a child to this layer
        this->addChild(label, 2);
    }

	auto spriteAir = Sprite::create("white8x8.png");
	spriteAir->setOpacity(255);
	spriteAir->setColor(Color3B(0,0,0));
	Vec2 airScale = Vec2(visibleSize.width * (1.0f / 8.0f), visibleSize.height*0.5f * (1.0f / 8.0f));
	spriteAir->setAnchorPoint(Vec2(0.5f, 0.0f));
	spriteAir->setScaleX(airScale.x);
	spriteAir->setScaleY(airScale.y);
	spriteAir->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height*0.5f));
	this->addChild(spriteAir, 1);

	auto spriteWater = Sprite::create("white8x8.png");
	spriteWater->setOpacity(255);
	spriteWater->setColor(Color3B(60, 191, 240));
	Vec2 waterScale = Vec2(visibleSize.width * (1.0f / 8.0f), 128.0f * (1.0f / 8.0f));
	spriteWater->setAnchorPoint(Vec2(0.5f, 1.0f));
	spriteWater->setScaleX(waterScale.x);
	spriteWater->setScaleY(waterScale.y);
	spriteWater->setPosition(Vec2(visibleSize.width * 0.5f, visibleSize.height*0.5f - 128.0f + 4.0f));
	this->addChild(spriteWater, 100);
	
	bShake = false;

//	pEntityManager->init();

	float globalOffsetY = -20.0f;

	// entity
	m_pSpritePlayer = Sprite::create("birdywalk1super64x64.png");
	m_pSpritePlayer->setVisible(false); // Hide Birdy sprite (We need to make a boat instead - TODO:)
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
	/*m_pSpriteMarker = Sprite::create("white8x8.png");
	if (m_pSpriteMarker != nullptr) {
		m_pSpriteMarker->setColor(Color3B(0, 0, 0));
		m_pSpriteMarker->setOpacity(96);
		m_pSpriteMarker->setScaleX(sx);
		m_pSpriteMarker->setScaleY(sy);
		Vec2 P = m_pSpritePlayer->getPosition();
		m_pSpriteMarker->setPosition(Vec2(P.x + markerOffsetX, P.y + visibleSize.height*0.5f + markerOffsetY + globalOffsetY));
		this->addChild(m_pSpriteMarker, 29);
	}*/

	//animFrames.reserve(2);
	//animFrames.pushBack(SpriteFrame::create("birdywalk1super64x64.png", Rect(0, 0, 64, 64)));
	//animFrames.pushBack(SpriteFrame::create("birdywalk2super64x64.png", Rect(0, 0, 64, 64)));

	//// create the animation out of the frames
	//animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	//animate = Animate::create(animation);

	// run it and repeat it forever
	//m_pSpritePlayer->runAction(RepeatForever::create(animate));

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

/*
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
//////////////////////////////////
// TODO: add this code back in - handles the letters creation!
	std::string french = "Bon  Bon  jour  bononoj  Bon  on  Jour  Jourouru  Bonjour";
	std::string s;// = std::string(1, french[3]);
	int length = french.size();
	float letterWidth = 12.0f;//16.0f;
	//cocos2d::log("french[3]: %c", french[3]);
	//cocos2d::log("s: %s", s.c_str());
	int m = -1;
	int i;
	for (i = 0; i < 3; i++) {
		for (n = 0; n <length; n++) {
			s = std::string(1, french[n]);
			if (s != " ") { // ignore white-space characters
				letter = new Letter(this);
				letter->spawn(s, i + 2, "white8x8.png", Vec2(200.0f + m * 320.0f + n*(letterWidth+4), visibleSize.height*0.55f - i * 60));
				pEntityManager->addEntity(letter);
			}
		}
	}
*/
	
	/*
	Floor* floor = nullptr;
	float width = visibleSize.width;
	float tileWidth = 64.0f;
	int floorTilesN = int(width / tileWidth) + 2; // 2 extra tiles needed minimum (1 for floor of value and 1 for seamless wrapping)
	for (n = 0; n < floorTilesN; n++) {
		floor = new Floor(this);
		floor->spawn("bodygreen64x64.png", Vec2(tileWidth*0.5f + n * 64.0f, tileWidth*0.5f + globalOffsetY));
		pEntityManager->addEntity(floor);
	}
	*/
	
	/*Sprite* spr = Sprite::create("./freeassets/image 1.png");
	spr->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	spr->setScaleX(2.0f);
	spr->setScaleY(1.5f);
	this->addChild(spr, 10);*/

	// create wave animation
	//animFrames.reserve(17);
	//char filename[128];
	//for (int i = 0; i < 17; i++) {
	//	sprintf(filename, "./freeassets/image %d.png", i + 1);
	//	cocos2d::log("filename : %s", filename);
	//	animFrames.pushBack(SpriteFrame::create(filename, Rect(0, 0, 256, 256)));
	//}
	//// create the animation out of the frames
	//animation = Animation::createWithSpriteFrames(animFrames, 0.05f);
	//animate = Animate::create(animation);

	// create wave sprites
	char filename[128];
	for (int j = 0; j < 3; j++) {
		sprintf(filename, "wave-%d-256x256.png", 1 + j);
		for (int i = 0; i < 6; i++) {
			waveSpr[j][i] = Sprite::create(filename);
			waveSpr[j][i]->setPosition(Vec2(visibleSize.width*0.5f + (i - 2) * 256, visibleSize.height*0.5f));
			this->addChild(waveSpr[j][i], 10-j);
			// run it and repeat it forever
			//if(i == 2)
			//waveSpr[i]->runAction(RepeatForever::create(animate->clone()));
		}
	}


	// Boat
	m_pSpriteBoat = Sprite::create("boat96x96.png");
	m_pSpriteBoat->setPosition(Vec2(visibleSize.width*0.2f, visibleSize.height*0.5f + 256*0.5f - 96*0.5f));
	this->addChild(m_pSpriteBoat, 10 - 4);

	// Man
	/*m_pSpriteMan = Sprite::create("man64x64.png");
	m_pSpriteMan->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	this->addChild(m_pSpriteMan, 150);*/

	// draw light beam!
	Point vertices[4] = { Vec2(0, 0), Vec2(-20, 220), Vec2(20, 220), Vec2(0, 0) };
	//DrawNode* polygon = DrawNode::create();
	m_pPolygon = DrawNode::create();
	//creating yellow polygon with invisible black border
	m_pPolygon->drawPolygon(vertices, 4, Color4F(1, 1, 0, 0.6f), 1, Color4F(0, 0, 0, 0));
	//polygon->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	m_pPolygon->setPosition(m_pSpriteBoat->getPosition());
	m_pPolygon->setRotation(180.0f);
	this->addChild(m_pPolygon, 200);

	// Next TODO:
	// Create separate wave in GIMP and makee them move at the different speeds!
	// Create boat (hopefully with polygons in code)
	// Create Lamp to highlight the words
	// Decide whether it's better to use letters of whole words


	// TODO: add factory method for creating different types of derived entities.

	// Create Boat Entity
	//std::vector<std::shared_ptr<Boat>> lst;
	//{
	//	std::shared_ptr<Boat> b1 = std::make_shared<Boat>();
	//	this->addChild(b1->spawn(), 200);

	//	lst.push_back(b1);
	//	lst[0].get()->pSprite->setScale(0.1f);
	//	cocos2d::log("INSIDE : lst[0].use_count() : %d", lst[0].use_count());
	//}
	//cocos2d::log("OUTSIDE : lst[0].use_count() : %d", lst[0].use_count());
	////WORKS!!!

	std::vector<Boat*> boatArr;
	for (int n = 0; n < 4; n++) {
		// create a boat and add to vector
		Vec2 boatPos = Vec2(visibleSize.width*0.5f + n*64.0f,
			visibleSize.height*0.5f + 256 * 0.5f - 96 * 0.5f);
		m_pBoat = Boat::create("boat96x96.png", boatPos);
		this->addChild(m_pBoat->pSprite, 300);
		m_pBoat->pSprite->setOpacity(192);
		m_pBoat->pSprite->setScale(0.5f);
		boatArr.push_back(m_pBoat);
	}
	cocos2d::log("boatArr.size() : %d", boatArr.size());
	
	for (std::vector<Boat*>::iterator it = boatArr.begin(); it != boatArr.end(); it++) {
		(*it)->pSprite->setRotation(float(floor(rand()%360)));
	}

	std::vector<int> v = { 10,20,30,40 };
	for (auto& i : v) {
		i++;
		cocos2d::log("i : %d", i);
	}

	for (auto i : v) {
		cocos2d::log("i : %d", i);
	}

	// TODO:
	// Add all the letters assets (aligned properly)
	// Add JSON letters.json that holds array of objects (letters) i.e. 
	// [
	//		{
	//			"icon": "A",
	//			"filename": "A.png",
	//			"width": 32,
	//			"height": 32,
	//			"offsetx": 1,
	//			"offsety": -2
	//		}
	// ]

	// create an icon "B"
	m_pIconB = Icon::create("B.png", Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	this->addChild(m_pIconB->pSprite, 200);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// create an icon string "Bonjour"
	//m_iconStringBonjour = IconString::create("Bonjour",
	//											Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f),
	//											this,
	//											200); // 1
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	m_iconStringBonjour = IconString::create(); // 2
	m_iconStringBonjour->setScale(0.5f);
	m_iconStringBonjour->spawn("Bonjour jour bon  bonjour", Vec2(100, 200), this, 200);
	std::vector<Icon*> iconArr = m_iconStringBonjour->spawn("non", Vec2(100, 200 - 50), this, 200);
	auto s = m_iconStringBonjour->spawnSprite("button-close-select64x64.png", Vec2(100, 200 - 100), this, 200);
	auto rot = RotateTo::create(1.0f, 360.0f);
	s->runAction(RepeatForever::create(rot));
	iconArr.at(8)->pSprite->setScale(0.5f);
	// make Bonjour effect
	int i = 0;
	for (Icon* pIcon : m_iconStringBonjour->iconArr) {
		if (i % 2 && i < 7) {
			//pIcon->pSprite->setVisible(false);
			pIcon->pSprite->setOpacity(64);
		}
		pIcon->pSprite->setColor(Color3B(20, 20, 20));
		i++;
	}
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//m_iconStringBonjour = IconString::create(); // 3
	//auto spr = m_iconStringBonjour->spawnSprite("B.png", Vec2(100, 100), this, 200);
	//m_iconStringBonjour->spawnSprite("o.png", Vec2(100 + 32, 100), this, 200);
	//m_iconStringBonjour->spawnSprite("n.png", Vec2(100 + 64, 100), this, 200);
	//spr->setRotation(45.0f);
	//m_iconStringBonjour->iconArr.at(1)->pSprite->setOpacity(128);
	//////////////////////////////////////////////////////////////////////////////////////////////////////


	/*cocos2d::log("m_boat.use_count() : %d", m_boat.use_count());
	{
		std::shared_ptr<Boat> boat = m_boat;
		boat->pSprite->setOpacity(128);
		cocos2d::log("m_boat.use_count() : %d", m_boat.use_count());
	}
	cocos2d::log("m_boat.use_count() : %d", m_boat.use_count());*/

	/*for (int i = 0; i < 4; i++) {
		std::shared_ptr<Boat> boat = std::make_shared<Boat>();
		this->addChild(boat->spawn());
		m_boatArr[i] = boat;
		cocos2d::log("PRE: m_boatArr[i].use_count() : %d", m_boatArr[i].use_count());
	}

	for (int i = 0; i < 4; i++) {
		cocos2d::log("POST: m_boatArr[i].use_count() : %d", m_boatArr[i].use_count());
	}*/


	/*int max = 3;
	m_pBoatArr = new std::shared_ptr<Boat>[max];
	for (int i = 0; i < 3; i++) {
		std::shared_ptr<Boat> boat = std::make_shared<Boat>();
		this->addChild(boat->spawn());
		m_pBoatArr[i] = boat;
		cocos2d::log("PRE: m_pBoatArr[i].use_count() : %d", m_pBoatArr[i].use_count());
	}

	for (int i = 0; i < 3; i++) {
		cocos2d::log("POST: m_pBoatArr[i].use_count() : %d", m_pBoatArr[i].use_count());
	}*/

	// not a good way!!!! Bad practice!
	//m_pBoatArr = new std::shared_ptr<Boat>;
	//m_pBoatArr->get()->sprite ...



	/*Vec2 boatPos = Vec2(visibleSize.width*0.2f,
						visibleSize.height*0.5f + 256 * 0.5f - 96 * 0.5f);
	m_pBoat = Boat::create("boat96x96.png", boatPos);
	this->addChild(m_pBoat->pSprite, 300);
	m_pBoat->pSprite->setOpacity(128);*/

	//Boat* gBoat = nullptr;
	//std::shared_ptr<Boat> gBoat;
	//
	//{
	//	//std::shared_ptr<Boat> gBoat = std::make_shared<Boat>();
	//	gBoat = std::make_shared<Boat>();
	//	gBoat->spawn("boat96x96.png", Vec2::ZERO);
	//	gBoat->pSprite->setPosition(Vec2::ZERO);
	//	this->addChild(gBoat->pSprite);
	//	//this->removeChild(pBoat->pSprite);

	//	//std::shared_ptr<Boat> pBoat2(gBoat);
	//	std::shared_ptr<Boat> pBoat2 = gBoat;
	//	m_boat = pBoat2;
	//	cocos2d::log("gBoat : use_count : %d", gBoat.use_count());
	//	cocos2d::log("pBoat2 : use_count : %d", pBoat2.use_count());
	//	
	//	//cocos2d::log("---");
	//	//auto myBoat = Boat::create("boat96x96.png");
	//	//myBoat->pSprite->setPosition(Vec2(visibleSize.width*0.2f, visibleSize.height*0.5f + 256 * 0.5f - 96 * 0.5f));
	//	//this->addChild(myBoat->pSprite, 10 - 4);
	//	//this->removeChild(myBoat->pSprite);
	//	//delete myBoat;
	//}

	//gBoat->pSprite->setRotation(45.0f);
	//if (gBoat != nullptr) cocos2d::log("gBoat alive!");
	//cocos2d::log("gBoat : use_count : %d", gBoat.use_count());

	// start update loop
	scheduleUpdate();

    return true;
}

void GameScene::update(float dt)
{
	//cocos2d::log("update...");

	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	if (m_pSpriteLazer->getScaleY() >= (visibleSize.height * (1.0f / 8.0f) *  0.75f)) // 90% of full  size
		bShake = true;
	
	shakeTheWorld(bShake);

//	pEntityManager->process();

	// move the water
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 6; i++) {
			if (waveSpr[j][i] != nullptr) {
				Vec2 pos = waveSpr[j][i]->getPosition();
				waveSpr[j][i]->setPosition(Vec2(visibleSize.width*0.5f + (i - 2) * 256 + wavePos[j].x,
					visibleSize.height*0.5f + wavePos[j].y));
			}
		}
		wavePos[j].x -= 4-j;
		if (wavePos[j].x < -256.0f) {
			wavePos[j].x += 256.0f;
		}
		wavePos[j].y = cos((m_seq + j*20.0f)*0.05f) * 4.0f;
	}

	// move the boat (up and down, rotate a little)
	float dy = cos((m_seq + 40.0f)*0.1f) * 6.0f;
	m_pSpriteBoat->setPosition(Vec2(visibleSize.width*0.2f, visibleSize.height*0.5f + 256 * 0.5f - 96 * 0.5f + dy));

	//float rnd = -5.0f + rand() % 11;
	float dr = cos((m_seq + 20.0f)*0.1f) * 8.0f;
	m_pSpriteBoat->setRotation(dr);

	// upddate light beam's position based on boat's position.
	m_pPolygon->setPosition(m_pSpriteBoat->getPosition());
	m_pPolygon->setRotation(180.0f + m_pSpriteBoat->getRotation() * 0.5f);
	// TODO: need to re-update the polygon's vertices to make more realistic!

	// Now destroy our icon string
	//if (m_cyc == 5 * 60) {
	//	cocos2d::log("m_iconStringBonjour.use_count() : %d", m_iconStringBonjour.use_count());
	//	m_iconStringBonjour.reset();
	//	cocos2d::log("m_iconStringBonjour.use_count() : %d", m_iconStringBonjour.use_count());
	//}

	m_seq++;
	m_cyc++;
}

void GameScene::shakeTheWorld(bool bStatus) {
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

void GameScene::menuCloseCallback(Ref* pSender)
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

void GameScene::menuPlayCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	// replace scene
	CGameManager::Instance()->m_stringGameType = "Game";
	CGameManager::Instance()->RunScene(kViewGame);
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("GameScene::onTouchBegan : You touched %f, %f", location.x, location.y);

	if (m_pSpritePlayer) {
		m_pSpritePlayer->stopAction(repeatSeqJelly);
		m_pSpritePlayer->stopAction(rotateToLeft);
		m_pSpritePlayer->stopAction(rotateToRight);
		rotateToShoot = RotateTo::create(0.3f, 0.0f);
		m_pSpritePlayer->runAction(rotateToShoot);

		// glowing
		tint = TintTo::create(0.2f, Color3B(192, 128, 128));
		m_pSpritePlayer->runAction(tint);
	}
	if (m_pSpriteLazer) {
		Letter::T.y = 0.0f; // indicates we are firing laser! >= 0
		
		// glowing lazer beam fx
		auto fadeTo = FadeTo::create(0.1f, 128.0f);
		auto fadeTo2 = FadeTo::create(0.1f, 64.0f);
		auto seqGlow = Sequence::create(fadeTo, fadeTo2, nullptr);
		m_pSpriteLazer->runAction(RepeatForever::create(seqGlow));
		
		// growing lazer beam fx
		auto scaleTo2 = ScaleTo::create(0.1f, 8.0f * 1.0f / 8.0f, visibleSize.height * 1.0f / 8.0f);
		m_pSpriteLazer->runAction(scaleTo2);
	}

	return true;
}

void GameScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
}

void GameScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("GameScene::onTouchEnded : You touched %f, %f", location.x, location.y);

	if (m_pSpritePlayer) {
		Letter::T.y = -1.0f; // indicates we are NOT firing laser! < 0
		m_pSpritePlayer->stopAction(rotateToShoot);
		rotateToLeft = RotateTo::create(0.07f, 6.0f);
		rotateToRight = RotateTo::create(0.07f, -6.0f);
		seqJelly = Sequence::create(rotateToLeft, rotateToRight, nullptr);
		repeatSeqJelly = RepeatForever::create(seqJelly);
		m_pSpritePlayer->runAction(repeatSeqJelly);

		// glowing
		m_pSpritePlayer->stopAction(tint);
		m_pSpritePlayer->setColor(Color3B(255, 255, 255));
	}

	if (m_pSpriteLazer) {
		bShake = false;
		m_pSpriteLazer->stopAllActions();
		m_pSpriteLazer->setScaleX(1.0f * 1.0f / 8.0f);
		m_pSpriteLazer->setScaleY(0.0f * 1.0f / 8.0f);
		
		// glowing lazer beam fx
		m_pSpriteLazer->stopAllActions();
		m_pSpriteLazer->setOpacity(0.0f);
	}
}
