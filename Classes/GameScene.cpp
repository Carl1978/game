#include "GameScene.h"
#include "SimpleAudioEngine.h"
#include "CGameManager.h"
#include "json/document.h"			// will include "rapidjson/rapidjson.h"
#include "json/writer.h"
#include "json/stringbuffer.h"
#include "json/filereadstream.h"   // FileReadStream
#include "json/encodedstream.h"    // EncodedInputStream
#include <sstream>
#include <iostream>
#include <string>
//#include <locale>
//#include <fcntl.h>
//#include <io.h>
//#include <cstring>

// Cocos2d-x Microphone Demonstration - Platfoamer (Part 2)
// https://www.youtube.com/watch?v=nfr-Wt9DHz0

// Free Assets (will add in credits)
// https://bayat.itch.io/platform-game-assets
// buttons made in GIMP: Brightness/Contrast +50
// 50x50 image resize, then canvas 64x64.

// Free licence with attribution
// https://www.freepik.com/free-vector/set-of-four-pretty-boats-of-colors_1108849.htm

// Free fonts .ttf
// https://www.fontspace.com/search/?q=bitmap

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#include <jni.h>
#endif

USING_NS_CC;
using namespace rapidjson;

float GameScene::currentVolume = 0;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern
"C"
{
	JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_sendVolume(JNIEnv* env, jobject obj, jfloat newVolume) {
		GameScene::currentVolume = newVolume;
	}
}
#endif

Vec2* Letter::wavePos;

GameScene::GameScene() {
	cocos2d::log("ctr...");

	Entity::currentID = 0;
	Floor::total = 0;
	Letter::T = Vec2(0, 0);
	idWordCurrent = -1;
	m_phraseKey = "";
	m_phraseVal = "";

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
	m_bShout = false;
	m_stateUpdate = 0;

	//for (int j = 0; j < 3; j++) {
	//	wavePos[j] = Vec2(0.0f, 0.0f);
	//	for (int i = 0; i < 6; i++) {
	//		waveSpr[j][i] = nullptr;
	//	}
	//}

	Letter::wavePos = &wavePos[0];
}

GameScene::~GameScene() {
	cocos2d::log("dtr...");

	if (m_pWaveArr.size()) {
		for (auto& pWave : m_pWaveArr) {
			if (pWave != nullptr) {
				delete pWave;
				pWave = nullptr;
			}
		}
	}

	// icon letters have to always be handled (NOTE: not an autorelease object)
	//if (m_iconLetters.size() > 0) {
	//	for (auto icon : m_iconLetters) {
	//		if (icon) {
	//			if (icon->pSprite) {
	//				icon->pSprite->release();
	//			}
	//			//	delete icon;
	//			//	icon = nullptr;
	//		}
	//	}
	//	//m_iconLetters.clear();
	//}

	//iconsRelease(m_iconLetters);
	//iconsRelease(m_iconStringQuestion->iconArr);
	//iconsRelease(m_iconStringAnswer->iconArr);
	//iconsRelease(m_iconStringBelt1->iconArr);
	//iconsRelease(m_iconStringBelt2->iconArr);
	//iconsRelease(m_iconStringBelt3->iconArr);

	if (repeatSeqJelly) repeatSeqJelly->release();
	if (seqJelly) seqJelly->release();
	if (rotateToLeft) rotateToLeft->release();
	if (rotateToRight) rotateToRight->release();

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
	if (!Scene::init())
	{
		return false;
	}

	// --------------------------------- ///

	m_documentLetters = parseJSON("json/letters.json");
	m_document = parseJSON("json/magyar.json");
	//m_document = parseJSON("json/magyar2.json");

	// debug
	//char buff[256];
	//sprintf(buff, "m_documentLetters['sprites'].GetString() : %s", m_documentLetters["sprites"].GetString());
	//cocos2d::log(buff);

	//const rapidjson::Value& a = m_documentLetters["sprites"];
	//for (rapidjson::Value::ConstMemberIterator iter = a.MemberBegin(); iter != a.MemberEnd(); ++iter) {
	//	const char *name = iter->name.GetString();
	//	std::string filename = iter->value.GetString();
	//	cocos2d::log("** name : %s : %s", name, filename.c_str());

		//if (strcmp(name, "id") == 0) {
		//	int id = iter->value.GetInt();
		//	cocos2d::log("value id : %d", id);
		//}
		//else if (strcmp(name, "type") == 0) {
		//	const char *type = iter->value.GetString();
		//	cocos2d::log("value type : %s", type);
		//}
		//else if (strcmp(name, "text") == 0) {
		//	const char *text = iter->value.GetString();
		//	cocos2d::log("value text : %s", text);
		//}
		//else if (strcmp(name, "lang") == 0) {
		//	const char *lang = iter->value.GetString();
		//	cocos2d::log("value lang : %s", lang);
		//}
		//else if (strcmp(name, "idlst") == 0) {
		//	// Using a reference for consecutive access is handy and faster.
		//	//const rapidjson::Value& a = _document["phrase1"];
		//	const rapidjson::Value& b = a["idlst"];
		//	//const rapidjson::Value& v = iter->value;
		//	assert(b.IsArray());
		//	for (SizeType i = 0; i < b.Size(); i++) // Uses SizeType instead of size_t
		//		cocos2d::log("value idlst : a[%d] = %d\n", i, b[i].GetInt());
		//}
	//}
	// --------------------------------- ///

	//Director::getInstance()->setClearColor(Color4F(0.0f, 1.0f, 0.0f, 0.0f));
	//LayerColor *_bgColor = LayerColor::create(Color4B(204, 230, 255, 255));
	//LayerColor *_bgColor = LayerColor::create(Color4B(0, 0, 0, 255));
	LayerColor *_bgColor = LayerColor::create(Color4B(60, 191, 240, 255));
	this->addChild(_bgColor, -10);

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
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2 - 5;
		float y = origin.y + visibleSize.height - closeItem->getContentSize().height / 2 - 5 - 2;
		closeItem->setPosition(Vec2(x, y));
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

	//auto label = Label::createWithTTF("1 + 1?", "fonts/arial.ttf", 24);
	//if (label == nullptr)
	//{
	//	problemLoading("'fonts/Marker Felt.ttf'");
	//}
	//else
	//{
	//	// position the label on the center of the screen
	//	label->setPosition(Vec2(origin.x + visibleSize.width / 2,
	//		origin.y + visibleSize.height - label->getContentSize().height));

	//	label->setTextColor(Color4B(255, 255, 255, 255));
	//	// add the label as a child to this layer
	//	this->addChild(label, 2);
	//}

	//////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////

	// Create star icon for score
	m_pSpriteStar = Sprite::create("star64x64.png");
	m_pSpriteStar->setPosition(Vec2(0.0f + 32.0f, visibleSize.height - 32.0f));
	m_pSpriteStar->setScale(0.5f);
	this->addChild(m_pSpriteStar, 300);

	// Create label for score
	TTFConfig config("fonts/PressStart2P.ttf", 14.0f);
	char buffer[128];
	score = 0;
	sprintf(buffer, "x %d", score);
	std::string sScore = std::string(buffer);
	pLabelScore = Label::createWithTTF(config, sScore, cocos2d::TextHAlignment::LEFT);
	pLabelScore->setTextColor(Color4B::WHITE);
	pLabelScore->enableOutline(Color4B::BLACK, 4);
	pLabelScore->setPosition(Vec2(40.0f + m_pSpriteStar->getContentSize().width * m_pSpriteStar->getScaleX() * 1.5f,
		visibleSize.height - 32.0f));
	this->addChild(pLabelScore, 300);

	// create sprite from this label test
	//TTFConfig configTest("fonts/PressStart2P.ttf", 24.0f);
	//Label* pLabelTest = Label::createWithTTF(configTest, "Hello!", cocos2d::TextHAlignment::CENTER);
	//pLabelTest->setTextColor(Color4B::WHITE);
	//pLabelTest->enableOutline(Color4B::BLACK, 6);
	//Sprite* pNewSprite = createSpriteFromLabel(pLabelTest);
	//pNewSprite->setPosition(Vec2(200.0f, 200.0f));
	//addChild(pNewSprite, 500);
	//cocos2d::log("pNewSprite->getPosition().x : %f", pNewSprite->getPosition().x);
	//cocos2d::log("pNewSprite->getContentSize().width : %f", pNewSprite->getContentSize().width);

	// get sprite letters from our json obj loaded in
	std::string stringLetters = m_documentLetters["alphabet"].GetString();
	stringLetters += m_documentLetters["numbers"].GetString();
	stringLetters += m_documentLetters["latin"].GetString();
	stringLetters += m_documentLetters["punctuation"].GetString();
	m_iconLetters = createIconArrFromString(stringLetters);

	Icon* pIcon = nullptr;
	const rapidjson::Value& a = m_documentLetters["sprites"];
	for (rapidjson::Value::ConstMemberIterator iter = a.MemberBegin(); iter != a.MemberEnd(); ++iter) {
		std::string name = iter->name.GetString();
		std::string filename = iter->value.GetString();
		cocos2d::log("** name : %s : %s", name.c_str(), filename.c_str());
		pIcon = Icon::create(filename, Vec2(100, 100));
		pIcon->value = name;
		//pSprite->setScale(0.6f);
		//P.x += pSprite->getContentSize().width * pSprite->getScale();
		////P.y += 5.0f;
		//pSprite->setPosition(P);
		//pSprite->setRotation(45.0f - rand() % 90);
		m_iconLetters.push_back(pIcon);
	}

	// add to parent layer & position them somewhere on screen
	/*Vec2 L = Vec2(32.0f, 90.0f);
	for (Icon* i : m_iconLetters) {
		if (i) {
			i->pSprite->setPosition(L + i->pSprite->getPosition());
			addChild(i->pSprite, 500);
		}
	}*/

	// TODO: build a word based on icons and then add them to a new iconstring object (converyor belt)
	// Useful for the wave moving icons / icons string
	//std::string alpha = m_document["magyar_letter"].GetString();//"z";//"*";//"W";
	//int idxLetter = getIdxFromIconValue(m_iconLetters, alpha);
	//m_pIconB = Icon::create(m_iconLetters.at(idxLetter)->value,
	//	m_iconLetters.at(idxLetter)->pSprite,
	//	Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	//this->addChild(m_pIconB->pSprite, 200);
	//cocos2d::log("m_pIconB->value : %s", m_pIconB->value.c_str());
	//cocos2d::log("m_pIconB->pSprite->getContentSize().width : %f", m_pIconB->pSprite->getContentSize().width);
	//cocos2d::log("m_pIconB->pSprite->getContentSize().height : %f", m_pIconB->pSprite->getContentSize().height);

	// Boat
	m_pBoat = Boat::create("boat96x96.png", Vec2(visibleSize.width*0.2f, visibleSize.height*0.5f + 256 * 0.5f - 96 * 0.5f));
	this->addChild(m_pBoat->pSprite, 10 - 4);

	// Man
	/*m_pSpriteMan = Sprite::create("man64x64.png");
	m_pSpriteMan->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	this->addChild(m_pSpriteMan, 150);*/

	// draw light beam!
	Point vertices[4] = { Vec2(0, 0), Vec2(-15, 220), Vec2(15, 220), Vec2(0, 0) };
	m_pPolygon = DrawNode::create();
	//creating yellow polygon with invisible black border
	m_pPolygon->drawPolygon(vertices, 4, Color4F(1, 1, 0, 0.4f), 1, Color4F(0, 0, 0, 0)); // was 0.6f
	m_pPolygon->setPosition(m_pBoat->pSprite->getPosition());
	m_pPolygon->setRotation(180.0f);
	this->addChild(m_pPolygon, 400);

	// draw light beam vert!
	//Point vert[4] = { Vec2(-1, 0), Vec2(-1, 220), Vec2(1, 220), Vec2(1, 0) };
	//m_pPolygonLine = DrawNode::create();
	////creating yellow polygon with invisible black border
	//m_pPolygonLine->drawPolygon(vert, 4, Color4F(0, 0, 0, 0.6f), 1, Color4F(0, 0, 0, 0)); // was 0.6f
	//m_pPolygonLine->setPosition(m_pBoat->pSprite->getPosition());
	//m_pPolygonLine->setRotation(180.0f);
	//this->addChild(m_pPolygonLine, 201);

	m_pPolygonLine = DrawNode::create();
	Vec2 centerPos = Vec2::ZERO;// m_pBoat->pSprite->getPosition();//Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f);
	constexpr int numPoints = 25;
	Vec2 pointArr[numPoints];
	for (int n = 0; n < numPoints; n++) {
		pointArr[n] = Vec2(centerPos.x, centerPos.y - 10.0f*n);
	}
	m_pPolygonLine->drawPoints(pointArr, numPoints, 2.0f, Color4F(0.0f, 0.0f, 0.0f, 0.9f));
	m_pPolygonLine->setPosition(m_pBoat->pSprite->getPosition());
	this->addChild(m_pPolygonLine, 199);
	//m_pPolygon->addChild(m_pPolygonLine);

	auto spriteAnchor = Sprite::create("anchor128x128.png");
	spriteAnchor->setOpacity(128);
	spriteAnchor->setScale(0.25f);
	spriteAnchor->setPosition(Vec2(0.0f, -10.0f * 25));
	m_pPolygonLine->addChild(spriteAnchor);

	/////////////////////////////////////////////////////////

	m_iconStringBelt1 = createBelt(Vec2(300.0f, 140.0f));
	m_iconStringBelt2 = createBelt(Vec2(0.0f, 140.0f + 64.0f));
	m_iconStringBelt3 = createBelt(Vec2(0.0f, 140.0f + 64.0f * 2.0f));

	//Icon* pI = nullptr;
	//for (int i = 0; i < m_iconStringBelt2->iconArr.size(); i++) {
	//	pI = m_iconStringBelt2->iconArr.at(i);
	//	if (pI) {
	//		pI->pSprite->setVisible(false);
	//	}
	//}

	//Icon* pI2 = nullptr;
	//for (int i = 0; i < m_iconStringBelt3->iconArr.size(); i++) {
	//	pI2 = m_iconStringBelt3->iconArr.at(i);
	//	if (pI2) {
	//		pI2->pSprite->setVisible(false);
	//	}
	//}

	// Useful for the questions at the top of screen.
	m_iconStringQuestion = IconString::create();
	m_iconStringQuestion->setScale(0.4f);

	std::string question = getRandomStringFromDocument(m_document);
	m_iconStringQuestion->spawn(m_iconLetters, question, Vec2(m_pPolygonLine->getPosition().x - 32.0f, visibleSize.height - 48.0f), this, 200);

	for (auto pIcon : m_iconStringQuestion->iconArr) {
		if (pIcon) {
			pIcon->pSprite->setColor(Color3B::YELLOW);
			addChild(pIcon->pSprite, 200);
		}
	}

	// builds up the answer string
	m_iconStringAnswer = IconString::create();
	m_iconStringAnswer->setScale(0.4f);
	//appendTextToIconString(m_iconStringAnswer, m_iconLetters, "Koszi szepen!", Vec2(m_pPolygonLine->getPosition().x - 32.0f, visibleSize.height * 0.5f), this, 200);
	//appendTextToIconString(m_iconStringAnswer, m_iconLetters, "Hello there", Vec2(m_pPolygonLine->getPosition().x - 32.0f, visibleSize.height * 0.5f), this, 200);
	// TODO: need to update above function to pass the iconstring var through
	// TODO: where we added the retain code to fix bug. We should also check the release on restart (maybe that's the memory leak / dangling pointers problem)

	/*appendTextToIconString(m_iconStringAnswer, m_iconLetters, "hi", Vec2(m_pPolygonLine->getPosition().x - 32.0f, visibleSize.height * 0.5f), this, 200);
	Texture2D* pText2 = m_iconLetters.at(0)->pSprite->getTexture();
	if (pText2) cocos2d::log("pText2 is valid!");*/

	// get string from our json obj loaded in
	//std::string stringComputer = m_document["computer"].GetString();
	//std::string stringComputer = m_document["magyar"].GetString();
	//std::vector<Sprite*> spriteComputerLst = createSpriteArrFromString(stringComputer);
	//// add to parent layer & position them somewhere on screen
	//Vec2 P = Vec2(64.0f, 150.0f);
	//for (Sprite* s : spriteComputerLst) {
	//	if (s) {
	//		s->setPosition(P + s->getPosition());
	//		addChild(s, 500);
	//	}
	//}
	/////////////////////////////////////////////////////////

	auto spriteAir = Sprite::create("white8x8.png");
	spriteAir->setOpacity(255);
	spriteAir->setColor(Color3B(0, 0, 0));
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

	// Create wave objects
	Wave* pWave = nullptr;
	char filename[128];
	for (int j = 0; j < 3; j++) {
		sprintf(filename, "wave-%d-256x256.png", 1 + j);
		for (int i = 0; i < Wave::waveSegmentCount; i++) {
			//waveSpr[j][i] = Sprite::create(filename);
			//waveSpr[j][i]->setPosition(Vec2(visibleSize.width*0.5f + (i - 2) * 256, visibleSize.height*0.5f));
			pWave = Wave::create(filename, Vec2(visibleSize.width*0.5f + (i - 2) * 256, visibleSize.height*0.5f), j);
			m_pWaveArr.push_back(pWave);
			this->addChild(pWave->pSprite, 10 - j);
			//this->addChild(waveSpr[j][i], 10 - j);
			// run it and repeat it forever
			//if(i == 2)
			//waveSpr[i]->runAction(RepeatForever::create(animate->clone()));
		}
	}

	// Next TODO:
	// Create separate wave in GIMP and make them move at the different speeds!
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

	//std::vector<Boat*> boatArr;
	//for (int n = 0; n < 4; n++) {
	//	// create a boat and add to vector
	//	Vec2 boatPos = Vec2(visibleSize.width*0.5f + n*64.0f,
	//		visibleSize.height*0.5f + 256 * 0.5f - 96 * 0.5f);
	//	m_pBoat = Boat::create("boat96x96.png", boatPos);
	//	this->addChild(m_pBoat->pSprite, 300);
	//	m_pBoat->pSprite->setOpacity(192);
	//	m_pBoat->pSprite->setScale(0.5f);
	//	boatArr.push_back(m_pBoat);
	//}
	//cocos2d::log("boatArr.size() : %d", boatArr.size());

	//for (std::vector<Boat*>::iterator it = boatArr.begin(); it != boatArr.end(); it++) {
	//	(*it)->pSprite->setRotation(float(floor(rand() % 360)));
	//}

	//std::vector<int> v = { 10,20,30,40 };
	//for (auto& i : v) {
	//	i++;
	//	cocos2d::log("i : %d", i);
	//}

	//for (auto i : v) {
	//	cocos2d::log("i : %d", i);
	//}

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
	//m_pIconB = Icon::create("B.png", Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	//this->addChild(m_pIconB->pSprite, 200);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	// create an icon string "Bonjour"
	//m_iconStringBonjour = IconString::create("Bonjour",
	//											Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f),
	//											this,
	//											200); // 1
	//////////////////////////////////////////////////////////////////////////////////////////////////////
// TODO: need to change this to the new system - check other todo in code
	//m_iconStringBonjour = IconString::create(); // 2
	//m_iconStringBonjour->setScale(0.5f);
	//m_iconStringBonjour->spawn("Bonjour jour bon  bonjour", Vec2(100, 200), this, 200);
	//std::vector<Icon*> iconArr = m_iconStringBonjour->spawn("non", Vec2(100, 200 - 50), this, 200);
	//auto s = m_iconStringBonjour->spawnSprite("button-close-select64x64.png", Vec2(100, 200 - 100), this, 200);
	//auto rot = RotateTo::create(1.0f, 360.0f);
	//s->runAction(RepeatForever::create(rot));
	//iconArr.at(8)->pSprite->setScale(0.5f);
	//// make Bonjour effect
	//int i = 0;
	//for (Icon* pIcon : m_iconStringBonjour->iconArr) {
	//	if (i % 2 && i < 7) {
	//		//pIcon->pSprite->setVisible(false);
	//		pIcon->pSprite->setOpacity(64);
	//	}
	//	pIcon->pSprite->setColor(Color3B(20, 20, 20));
	//	i++;
	//}
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

	// add additional display values (Mic debug)
	micLabel = Label::create("000000.000000", "fonts/arial.ttf", 18);
	micLabel->setColor(Color3B(0, 0, 0));
	micLabel->setAnchorPoint(Vec2(0.0f, 0.5f));
	micLabel->setPosition(Vec2(origin.x + visibleSize.width - micLabel->getContentSize().width - 16.0f, origin.y + micLabel->getContentSize().height * 2));
	this->addChild(micLabel, 500);

	// start update loop
	scheduleUpdate();

	return true;
}

void GameScene::update(float dt)
{
	//cocos2d::log("update...");

	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	// Update mic debug
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	micLabel->setString(std::to_string(GameScene::currentVolume));

	//float volumeHeight = (GameScene::currentVolume / 4000.0f) * 256;

	if (m_stateUpdate == 0) {
		if (GameScene::currentVolume > 100.0f) {
			processShout(GameScene::currentVolume);
		}
	}
	else if (m_stateUpdate == 1) {
		if (GameScene::currentVolume <= 100.0f) {
			processShout(GameScene::currentVolume);
		}
	}

	//if (pSpriteVolumeLevel) {
	//	// assume microphone range is between 0 - 4000
	//	if (volumeHeight < 0) volumeHeight = 0;
	//	else if (volumeHeight > 256) volumeHeight = 256;
	//	pSpriteVolumeLevel->setTextureRect(Rect(0, 0, 8, int(volumeHeight)));
	//}

	//// Smooth volume i.e. decays from it's current value at a slower rate.
	//// and if it's over a certain threshold then set flag!
	//if (myPhraseManager) {
	//	// make sure we are not overriding by pressing the fake voice button?
	//	if (pSpriteSpeakOn->isVisible() == false) {
	//		volumeDecay = 0.8;
	//		if (GameLayer::currentVolume >= 90) {
	//			volumeThreshold = GameLayer::currentVolume;
	//		}
	//		else {
	//			volumeThreshold *= volumeDecay;
	//		}

	//		if (volumeThreshold >= 80) {
	//			myPhraseManager->bFragmentSpeak = true;
	//		}
	//		else {
	//			myPhraseManager->bFragmentSpeak = false;
	//		}
	//	}
	//}
#endif

	// move the water
	for (auto& wave : m_pWaveArr) {
		if (wave != nullptr) wave->process();
	}

	// move the boat (up and down, rotate a little)
	if (m_pBoat != nullptr) m_pBoat->process();

	// process conveyor belts
	processPositionIconStringToWave(m_iconStringBelt1, m_pWaveArr[Wave::waveSegmentCount * 0]);
	processPositionIconStringToWave(m_iconStringBelt2, m_pWaveArr[Wave::waveSegmentCount * 1]);
	processPositionIconStringToWave(m_iconStringBelt3, m_pWaveArr[Wave::waveSegmentCount * 2]);

	if (!m_bShout) {
		// upddate light beam's position based on boat's position.
		m_pPolygon->setPosition(m_pBoat->pSprite->getPosition());
		float damp = 0.25f;//0.5f;

		m_pPolygon->setRotation(180.0f + m_pBoat->pSprite->getRotation() * damp);
		// TODO: need to re-update the polygon's vertices to make more realistic!
		m_pPolygonLine->setPosition(m_pBoat->pSprite->getPosition());
		//m_pPolygonLine->setRotation(180.0f + m_pBoat->pSprite->getRotation() * 0.1f);
	}
	else {
		int idWord = getLightBeamIconStringWord(m_pPolygonLine, m_iconStringBelt1);
		if (idWord != -1) {
			if (idWord != idWordCurrent) { // found a different word
				std::string word = m_iconStringBelt1->m_words.at(idWord);
				if (m_stateUpdate == 1) {
					//m_stateUpdate = 2;
					idWordCurrent = idWord;
					int prevSize = m_iconStringAnswer->iconArr.size();
					appendTextToIconString(m_iconStringAnswer, m_iconLetters, word,
						Vec2(m_pPolygonLine->getPosition().x - 32.0f, visibleSize.height * 0.5f), this, 200);
				}
			}
		}

		// make light beam shine on a word
		processLightBeamIconStringWord(m_pPolygonLine, m_iconStringBelt1);
		//processLightBeamIconStringWord(m_pPolygonLine, m_iconStringBelt2);
		//processLightBeamIconStringWord(m_pPolygonLine, m_iconStringBelt3);
	}

	// position question & answer to anchor / light-beam
	Vec2 P = m_pPolygonLine->getPosition();
	P.y = P.y - 10.0f*25.0f;// m_pPolygonLine->getContentSize().height;
	processPositionIconStringToPos(m_iconStringQuestion, P);
	processPositionIconStringToPos(m_iconStringAnswer, P, Vec2(0.0f, 24.0f));

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

void GameScene::processShout(float volume) {
	if (volume > 100.0f) {
		m_bShout = true;
		m_stateUpdate = 1;

		// slow down the waves now that we are shouting / touching the screen!
		for (auto& wave : m_pWaveArr) {
			if (wave != nullptr) {
				wave->vel *= 0.5f;
			}
		}
	}
	else {
		m_bShout = false;
		m_stateUpdate = 0;

		// check if we have the correct answer? i.e. matching phrase between the answer icon string
		Size visibleSize = Director::getInstance()->getVisibleSize();

		// create the phrase key
		cocos2d::log("m_iconStringQuestion->m_words.size() : %d", m_iconStringQuestion->m_words.size());
		for (int i = 0; i < m_iconStringQuestion->m_words.size(); i++) {
			cocos2d::log("m_iconStringQuestion->m_words.at(i) : %s", m_iconStringQuestion->m_words.at(i).c_str());
			if (m_phraseKey != "") {
				m_phraseKey += " ";
			}
			m_phraseKey += m_iconStringQuestion->m_words.at(i);
		}

		// create the phrase value
		cocos2d::log("m_iconStringAnswer->m_words.size() : %d", m_iconStringAnswer->m_words.size());
		for (int i = 0; i < m_iconStringAnswer->m_words.size(); i++) {
			cocos2d::log("m_iconStringAnswer->m_words.at(i) : %s", m_iconStringAnswer->m_words.at(i).c_str());
			if (m_phraseVal != "") {
				m_phraseVal += " ";
			}
			m_phraseVal += m_iconStringAnswer->m_words.at(i);
		}

		cocos2d::log("m_phraseKey : %s", m_phraseKey.c_str());
		cocos2d::log("m_phraseVal : %s", m_phraseVal.c_str());

		std::string value = m_document[m_phraseKey.c_str()].GetString();
		cocos2d::log("value : %s", value.c_str());

		if (m_phraseVal == value) {
			cocos2d::log("Correct Answer!!! :)");
			updateScore(++score);
		}
		else {
			cocos2d::log("Wrong Answer! :(");
			if (score > 0) score--;
			updateScore(score);
		}

		m_phraseKey = "";
		m_phraseVal = "";

		// rebuilds up the question string
		for (auto pIcon : m_iconStringQuestion->iconArr) {
			if (pIcon) {
				//delete pIcon; // NOTE: don't need to delete, handle automatically when we pop or clear the vector array
				//pIcon = nullptr;
				this->removeChild(pIcon->pSprite);
			}
		}

		m_iconStringQuestion->iconArr.clear();
		m_iconStringQuestion->m_words.clear();
		std::string question = getRandomStringFromDocument(m_document);
		m_iconStringQuestion->spawn(m_iconLetters, question, Vec2(m_pPolygonLine->getPosition().x - 32.0f, visibleSize.height - 48.0f), this, 200);
		for (auto pIcon : m_iconStringQuestion->iconArr) {
			if (pIcon) {
				pIcon->pSprite->setColor(Color3B::YELLOW);
				addChild(pIcon->pSprite, 200);
			}
		}

		// rebuilds up the answer string
		for (auto pIcon : m_iconStringAnswer->iconArr) {
			if (pIcon) {
				//delete pIcon; // NOTE: don't need to delete, handle automatically when we pop or clear the vector array
				//pIcon = nullptr;
				this->removeChild(pIcon->pSprite);
			}
		}

		m_iconStringAnswer->iconArr.clear();
		m_iconStringAnswer->m_words.clear();

		// draw light beam!
		Point vertices[4] = { Vec2(0, 0), Vec2(-15, 220), Vec2(15, 220), Vec2(0, 0) };
		m_pPolygon->clear();
		m_pPolygon->drawPolygon(vertices, 4, Color4F(1, 1, 0, 0.4f), 1, Color4F(0, 0, 0, 0)); // was 0.6f
		m_pPolygon->setPosition(m_pBoat->pSprite->getPosition());
		m_pPolygon->setRotation(180.0f);

		// speed up the waves again!
		for (auto& wave : m_pWaveArr) {
			if (wave != nullptr) {
				wave->vel *= (1.0f / 0.5f);
			}
		}
	}
}

bool GameScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("GameScene::onTouchBegan : You touched %f, %f", location.x, location.y);

	if (m_stateUpdate == 0) {
		GameScene::currentVolume = 1000.0f;
		processShout(GameScene::currentVolume);
	}

	// NOTE: very important!! Speed should accelerate and de-accellerate, will feel more smooth for user especially when speaking!
	// Also, we will need tto be able to calibrate this I think in settings!
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

	if (m_stateUpdate == 1) {
		GameScene::currentVolume = 0.0f;
		processShout(GameScene::currentVolume);
	}

	//if (m_pSpritePlayer) {
	//	Letter::T.y = -1.0f; // indicates we are NOT firing laser! < 0
	//	m_pSpritePlayer->stopAction(rotateToShoot);
	//	rotateToLeft = RotateTo::create(0.07f, 6.0f);
	//	rotateToRight = RotateTo::create(0.07f, -6.0f);
	//	seqJelly = Sequence::create(rotateToLeft, rotateToRight, nullptr);
	//	repeatSeqJelly = RepeatForever::create(seqJelly);
	//	m_pSpritePlayer->runAction(repeatSeqJelly);

	//	// glowing
	//	m_pSpritePlayer->stopAction(tint);
	//	m_pSpritePlayer->setColor(Color3B(255, 255, 255));
	//}

	//if (m_pSpriteLazer) {
	//	bShake = false;
	//	m_pSpriteLazer->stopAllActions();
	//	m_pSpriteLazer->setScaleX(1.0f * 1.0f / 8.0f);
	//	m_pSpriteLazer->setScaleY(0.0f * 1.0f / 8.0f);

	//	// glowing lazer beam fx
	//	m_pSpriteLazer->stopAllActions();
	//	m_pSpriteLazer->setOpacity(0.0f);
	//}
}

Sprite* GameScene::createSpriteFromLabel(Label* pLabel) {
	if (pLabel == nullptr) { return nullptr; }

	Size pixelSize = Director::getInstance()->getWinSizeInPixels();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Size letterSize = pLabel->getContentSize() + Size(pLabel->getOutlineSize()*2.0f, pLabel->getOutlineSize()*2.0f);
	Vec2 labelPrevPos = pLabel->getPosition();
	float labelPrevScaleY = pLabel->getScaleY();
	RenderTexture* pRenderTexture;
	Sprite* pSprite = nullptr;
	Sprite* pSpriteContainer = Sprite::create();

	pRenderTexture = RenderTexture::create(letterSize.width, letterSize.height, Texture2D::PixelFormat::RGBA8888);
	pRenderTexture->retain();
	pRenderTexture->setKeepMatrix(true);
	pRenderTexture->setVirtualViewport(Vec2(0, 0),
		Rect(0, 0, visibleSize.width, visibleSize.height),
		Rect(0, 0, pixelSize.width, pixelSize.height));

	pSpriteContainer->addChild(pLabel, 0);
	pLabel->setPosition(Vec2(letterSize.width*0.5f, letterSize.height*0.5f));
	pLabel->setScaleY(-1.0f);

	pRenderTexture->beginWithClear(1, 1, 1, 0);
	pSpriteContainer->visit();
	pRenderTexture->end();
	//pRenderTexture->saveToFile("test.png", Image::Format::PNG);

	pLabel->setScaleY(labelPrevScaleY);
	pLabel->setPosition(labelPrevPos);
	pSpriteContainer->removeChild(pLabel, false);

	pSprite = Sprite::createWithTexture(pRenderTexture->getSprite()->getTexture());
	pSprite->retain();
	//pRenderTexture->retain(); // NOTE: the below comment retains made it slow down alot each time I clicked the restart button in app!!!
	//pRenderTexture->getSprite()->retain();
	//pRenderTexture->getSprite()->getTexture()->retain();
	//usa
	//pSprite = Sprite::create();
	//pSprite->setTexture("button-close64x64.png");
	//pSprite = Sprite::create("button-close64x64.png");
	//pSprite->getTexture()->retain();//usa
	//pSprite->initWithTexture(pRenderTexture->getSprite()->getTexture());
	//pSprite->createWithSpriteFrame(pRenderTexture->getSprite()->getSpriteFrame());

	pRenderTexture->release();

	pSprite->setContentSize(letterSize);

	return pSprite;
}

std::vector<Sprite*> GameScene::createSpriteArrFromLabel(Label* pLabel, const std::string& text) {
	std::vector<Sprite*> spriteArr;
	if (pLabel == nullptr) { return spriteArr; }

	pLabel->setString(text);

	Size pixelSize = Director::getInstance()->getWinSizeInPixels();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Size letterSize = pLabel->getContentSize() + Size(pLabel->getOutlineSize(), pLabel->getOutlineSize());
	Vec2 labelPrevPos = pLabel->getPosition();
	float labelPrevScaleY = pLabel->getScaleY();
	RenderTexture* pRenderTexture;
	Sprite* pSprite = nullptr;
	Sprite* pSpriteContainer = Sprite::create();

	pRenderTexture = RenderTexture::create(letterSize.width, letterSize.height, Texture2D::PixelFormat::RGBA8888);
	pRenderTexture->retain();
	pRenderTexture->setKeepMatrix(true);
	pRenderTexture->setVirtualViewport(Vec2(0, 0),
		Rect(0, 0, visibleSize.width, visibleSize.height),
		Rect(0, 0, pixelSize.width, pixelSize.height));

	pSpriteContainer->addChild(pLabel, 0);
	pLabel->setPosition(Vec2(letterSize.width*0.5f, letterSize.height*0.5f));
	pLabel->setScaleY(-1.0f);

	pRenderTexture->beginWithClear(1, 1, 1, 0);
	pSpriteContainer->visit();
	pRenderTexture->end();

	pLabel->setScaleY(labelPrevScaleY);
	pLabel->setPosition(labelPrevPos);
	pSpriteContainer->removeChild(pLabel, false);

	pSprite = Sprite::createWithTexture(pRenderTexture->getSprite()->getTexture());

	pRenderTexture->release();

	spriteArr.push_back(pSprite);

	return spriteArr;
}

std::vector<Sprite*> GameScene::createSpriteArrFromString(const std::string& text) {
	std::vector<Sprite*> spriteArr;
	Sprite* pSprite = nullptr;
	Label* pLabel = nullptr;
	// Create a label with given font
	TTFConfig config("fonts/Good Unicorn - TTF.ttf", 64.0f);
	std::vector<std::string> sArr;
	size_t size = text.size();
	if (size <= 0) { return spriteArr; }

	// special I wrote to check if we have an ASCII char or Unicode char (i.e. 2bytes)
	// NOTE: in some cases this can be 32-bit 4bytes so may have to come back and add platform checking code here later!
	// TODO: refactor into a nice neat helper function
	for (int pos = 0; pos < size;)
	{
		unsigned int c = text.at(pos);
		cocos2d::log("%c, %d", c, c);
		// single char (in ASCII range)
		if (c < 128) {
			std::string s = text.substr(pos, 1);
			sArr.push_back(s);
			cocos2d::log("s: %s", s.c_str());
			pos++;
		}
		// double char needed (unicode range)
		else {
			std::string s = text.substr(pos, 2);
			sArr.push_back(s);
			cocos2d::log("s: %s", s.c_str());
			pos += 2;
		}
	}

	Vec2 P = Vec2(0.0f, 0.0f);
	for (int i = 0; i < sArr.size(); i++) {
		if (sArr[i].at(0) != 32) { // check we have a valid char (not space " " char)
			pLabel = Label::createWithTTF(config, sArr[i].c_str(), cocos2d::TextHAlignment::CENTER);
			pLabel->setTextColor(Color4B::WHITE);
			pLabel->enableOutline(Color4B::BLACK, 6);

			// create sprite from this label
			pSprite = createSpriteFromLabel(pLabel);
			pSprite->setScale(0.6f);
			P.x += pSprite->getContentSize().width * pSprite->getScale();
			//P.y += 5.0f;
			pSprite->setPosition(P);
			pSprite->setRotation(45.0f - rand() % 90);
			spriteArr.push_back(pSprite);
		}
		else {
			// for now assume space chars are equal to this width
			P.x += 20.0f;
		}
	}

	return spriteArr;
}

//std::vector<Icon*> iconArr;
std::vector<Icon*> GameScene::createIconArrFromString(const std::string& text) {
	std::vector<Icon*> iconArr;
	Sprite* pSprite = nullptr;
	Icon* pIcon = nullptr;
	Label* pLabel = nullptr;
	// Create a label with given font
	//TTFConfig config("fonts/Good Unicorn - TTF.ttf", 64.0f);
	TTFConfig config("fonts/PressStart2P.ttf", 24.0f);//30.0f);//24.0f);//usa
	std::vector<std::string> sArr;
	size_t size = text.size();
	if (size <= 0) { return iconArr; }

	// special I wrote to check if we have an ASCII char or Unicode char (i.e. 2bytes)
	// NOTE: in some cases this can be 32-bit 4bytes so may have to come back and add platform checking code here later!
	// TODO: refactor into a nice neat helper function
	for (int pos = 0; pos < size;)
	{
		unsigned int c = text.at(pos);
		cocos2d::log("%c, %d", c, c);
		// single char (in ASCII range)
		if (c < 128) {
			std::string s = text.substr(pos, 1);
			sArr.push_back(s);
			cocos2d::log("s: %s", s.c_str());
			pos++;
		}
		// double char needed (unicode range)
		else {
			std::string s = text.substr(pos, 2);
			sArr.push_back(s);
			cocos2d::log("s: %s", s.c_str());
			pos += 2;
		}
	}

	Vec2 P = Vec2(0.0f, 0.0f);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	for (int i = 0; i < sArr.size(); i++) {
		if (sArr[i].at(0) != 32) { // check we have a valid char (not space " " char)
			pLabel = Label::createWithTTF(config, sArr[i].c_str(), cocos2d::TextHAlignment::CENTER);
			pLabel->setTextColor(Color4B::WHITE);
			pLabel->enableOutline(Color4B::BLACK, 6);

			// create sprite from this label
			pSprite = createSpriteFromLabel(pLabel);
			//pSprite->setScale(0.6f);
			P.x += pSprite->getContentSize().width * pSprite->getScale();
			//P.y += 5.0f;
			pSprite->setPosition(P);
			//pSprite->setRotation(45.0f - rand() % 90);

			pIcon = Icon::create(sArr[i], pSprite, Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));

			iconArr.push_back(pIcon);
		}
		else {
			// for now assume space chars are equal to this width
			P.x += 20.0f;
		}
	}

	return iconArr;
}

rapidjson::Document GameScene::parseJSON(const std::string& filename) {
	rapidjson::Document m_doc; // Document is GenericDocument<UTF8<> >

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	// Works - reads correct json file UTF8 (ANSI)
	//auto fileData = FileUtils::getInstance()->getDataFromFile("example.json");
	//std::string content((const char *)fileData.getBytes(), fileData.getSize());
	//m_doc.Parse<0>(content.c_str());
	//assert(m_doc.IsObject());
	//assert(m_doc.HasMember("hello"));
	//assert(m_doc["hello"].IsString());

	// Works - reads correct json file UTF16 (UNICODE)
	auto fileData = FileUtils::getInstance()->getDataFromFile(filename);
	std::string content((const char *)fileData.getBytes(), fileData.getSize());

	StringStream bis(content.c_str());
	EncodedInputStream<UTF16LE<>, StringStream> eis(bis); // wraps bis into eis
	m_doc.ParseStream<0, UTF16LE<> >(eis);  // Parses UTF-16 file into UTF-8 in memory

	//assert(m_doc.IsObject());
	//assert(m_doc.HasMember("hello"));
	//assert(m_doc["hello"].IsString());
#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	// Works - reads correct json file UTF8 (ANSI)
	//FILE *fp = fopen("example.json", "rb"); // non-Windows use "r"
	//char readBuffer[65536]; // 64K
	//FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	//m_doc.ParseStream(is);
	//fclose(fp);

	// Works - reads correct json file UTF16 (UNICODE)
	FILE *fp = fopen(filename.c_str(), "rt+, ccs=UNICODE");
	char readBuffer[65536]; // 64K
	FileReadStream bis(fp, readBuffer, sizeof(readBuffer));
	EncodedInputStream<UTF16LE<>, FileReadStream> eis(bis);  // wraps bis into eis
	m_doc.ParseStream<0, UTF16LE<> >(eis);  // Parses UTF-16 file into UTF-8 in memory
	fclose(fp);
#endif

	return m_doc;
}

int GameScene::getIdxFromIconValue(std::vector<Icon*> iconLetters, const std::string value) {
	// create an icon "A" from sprite letters vector created above
	Icon* pI = nullptr;
	for (int i = 0; i < iconLetters.size(); i++) {
		pI = iconLetters.at(i);
		if (pI != nullptr) {
			if (pI->value == value) {
				return i;
			}
		}
	}
	return -1;
}

// TODO: wave should be an object i.e. Wave* pWave
void GameScene::processPositionIconStringToWave(std::shared_ptr<IconString> iconString, Wave* pWave) {
	if (iconString == nullptr || iconString->iconArr.size() <= 0) return;

	Vec2 vel = Vec2::ZERO;
	int waveIdx = 0;
	if (pWave != nullptr) {
		vel = pWave->vel;
		waveIdx = pWave->idx;
	}

	Vec2 P;
	for (Icon* pIcon : iconString->iconArr) {
		P = pIcon->pos;
		P.x -= vel.x;
		// TODO: wrap-around should be based on the total length of text icon string, but will change dynamically
		if (P.x < -256.0f) {
			P.x += (256.0f + iconString->width);
		}
		P.y = pIcon->startPos.y + cos((m_seq + waveIdx*20.0f)*0.05f) * 4.0f;

		pIcon->pos = P;
		pIcon->pSprite->setPosition(P);
	}
}

void GameScene::processPositionIconStringToPos(std::shared_ptr<IconString> iconString, Vec2 pos, Vec2 offset) {
	if (iconString == nullptr || iconString->iconArr.size() <= 0) return;

	Vec2 P;
	for (Icon* pIcon : iconString->iconArr) {
		P = pIcon->pos;
		P.y = pos.y;
		pIcon->pos = P;
		pIcon->pSprite->setPosition(P + offset);
	}
}

void GameScene::processLightBeamIconStringWord(DrawNode* m_pLightBeam, std::shared_ptr<IconString> iconString) {
	// find word
	Vec2 T = m_pPolygonLine->getPosition();
	Vec2 P;
	float d = 0.0f;
	Icon* pIcon = nullptr;
	Icon* pIconFound = nullptr;
	int idxFound = -1;
	float x = 0.0f;
	float width = 0.0f;
	float scale = 1.0f;

	// finds the first icon (letter) and then  anything on the RHS
	for (int i = 0; i < iconString->iconArr.size(); i++) {
		pIcon = iconString->iconArr.at(i);
		P = pIcon->pSprite->getPosition();
		width = pIcon->pSprite->getContentSize().width;
		scale = pIcon->pSprite->getScale();
		d = fabs(T.x - P.x);
		if (d < width * scale * 0.5f) {
			//pIcon->pSprite->setOpacity(128);
			pIconFound = pIcon;
			idxFound = i;
			//cocos2d::log("pIconFound->idWord : %d", pIconFound->idWord);
			break;
		}
	}

	// calculate and update the light beam onto this word
	if (pIconFound != nullptr) {
		// draw light beam on full!
		float paddingBottom = 12.0f;
		float paddingLeftRight = 4.0f;
		float x = pIcon->pSprite->getPosition().x;
		Vec2 leftPos = pIcon->pSprite->getPosition();
		Vec2 rightPos = pIcon->pSprite->getPosition();

		leftPos.x = T.x - leftPos.x + (width * scale * 0.5f + paddingLeftRight);
		rightPos.x = T.x - rightPos.x - (width * scale * 0.5f + paddingLeftRight);
		leftPos.y = m_pBoat->pSprite->getPosition().y - leftPos.y + paddingBottom;
		rightPos.y = m_pBoat->pSprite->getPosition().y - rightPos.y + paddingBottom;

		Point vertices[4] = { Vec2(0, 0), Vec2(rightPos.x, leftPos.y), Vec2(leftPos.x, rightPos.y), Vec2(0, 0) };

		m_pPolygon->clear();
		m_pPolygon->drawPolygon(vertices, 4, Color4F(1, 1, 0, 0.6f), 1, Color4F(0, 0, 0, 0));
		m_pPolygon->setPosition(m_pBoat->pSprite->getPosition());
		m_pPolygon->setRotation(180.0f);
	}
	//else {
		// draw light beam!
		//Point vertices[4] = { Vec2(0, 0), Vec2(-15, 220), Vec2(15, 220), Vec2(0, 0) };
		//m_pPolygon->clear();
		//m_pPolygon->drawPolygon(vertices, 4, Color4F(1, 1, 0, 0.5f), 1, Color4F(0, 0, 0, 0)); // was 0.6f
		//m_pPolygon->setPosition(m_pBoat->pSprite->getPosition());
		//m_pPolygon->setRotation(180.0f);
	//}
}

int GameScene::getLightBeamIconStringWord(DrawNode* m_pLightBeam, std::shared_ptr<IconString> iconString) {
	// find word
	Vec2 T = m_pPolygonLine->getPosition();
	Vec2 P;
	float d = 0.0f;
	Icon* pIcon = nullptr;
	int idWord = -1;

	// finds the first icon (letter) and then returns the idWord of that icon
	for (int i = 0; i < m_iconStringBelt1->iconArr.size(); i++) {
		pIcon = m_iconStringBelt1->iconArr.at(i);
		P = pIcon->pos;
		d = fabs(T.x - P.x);
		if (d < 16.0f) {
			idWord = pIcon->idWord;
			//cocos2d::log("getLightBeamIconStringWord : idWord : %d", idWord);
			break;
		}
	}

	return idWord;
}

void GameScene::appendTextToIconString(std::shared_ptr<IconString> iconString, std::vector<Icon*> icons, const std::string text, Vec2 P, Scene* pSceneParent, int localZOrder) {
	float endPos = iconString->getwidth() ? iconString->getwidth() + 60.0f * iconString->getScale() : 0.0f;
	int size = iconString->iconArr.size();
	iconString->spawn(m_iconLetters, text, Vec2(P.x + endPos, P.y), this, 200);
	Icon *pI = nullptr;
	for (int n = size; n < iconString->iconArr.size(); n++) {
		pI = iconString->iconArr[n];
		if (pI) {
			pI->pSprite->setColor(Color3B::YELLOW);
			addChild(pI->pSprite, 200);
		}
	}
}

std::string GameScene::getRandomStringFromDocument(const rapidjson::Document& doc) {
	rapidjson::SizeType size = doc.MemberCount();
	int randIdx = rand() % size;
	int i = 0;
	for (rapidjson::Value::ConstMemberIterator iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter) {
		if (i == randIdx) {
			std::string name = iter->name.GetString();
			//std::string text = iter->value.GetString();
			return name;
		}
		i++;
	}
	return "";
}

void GameScene::updateScore(int newScore) {
	char buffer[128];
	sprintf(buffer, "x %d", newScore);
	std::string sScore = std::string(buffer);
	pLabelScore->setString(sScore);
}

void GameScene::iconsRelease(std::vector<Icon*> icons) {
	if (icons.size() > 0) {
		for (auto icon : icons) {
			if (icon) {
				if (icon->pSprite) {
					icon->pSprite->release();
				}
			}
		}
	}
}

std::shared_ptr<IconString> GameScene::createBelt(Vec2 pos) {
	std::shared_ptr<IconString> iconString = IconString::create();
	iconString->setScale(0.5f);

	std::string msg = "";
	const rapidjson::Value& doc = m_document;
	for (rapidjson::Value::ConstMemberIterator iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter) {
		std::string name = iter->name.GetString();
		std::string text = iter->value.GetString();
		//cocos2d::log("** name : text => %s : %s", name.c_str(), text.c_str());
		//m_phrases.push_back(text);
		msg += text;
		msg += " ";
	}

	iconString->spawn(m_iconLetters, msg, pos, this, 200);

	for (Icon* pIcon : iconString->iconArr) {
		if (pIcon) {
			addChild(pIcon->pSprite, 200);
		}
	}

	return iconString;
}