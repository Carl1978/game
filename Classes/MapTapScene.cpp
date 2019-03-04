#include "MapTapScene.h"
#include "SimpleAudioEngine.h"
#include "CGameManager.h"
#include <vector>
#include <string>
#include <limits>
#include <functional>

USING_NS_CC;

MapTapScene::MapTapScene() {
	cocos2d::log("MapTapScene :: ctr...");

	// produce true random numbers?!
	//time_t theTime;
	srand(time(NULL));
	//srand(time(&theTime));

	//int i;
	//for (i = 0; i < 100; i++) {
	//	cocos2d::log("rand : i = %d : %d", i, rand()%10);
	//}

	m_pCamera = nullptr;

	m_seq = 0.0f;
	m_cyc = 0;
	m_R = Vec2(0.0f, -1.0f);
}

MapTapScene::~MapTapScene() {
	cocos2d::log("MapTapScene :: dtr...");

	// remove cameras
	if (m_pCamera != nullptr) {
		this->removeChild(m_pCamera);
		m_pCamera = nullptr;
	}
}

Scene* MapTapScene::createScene()
{
	return MapTapScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in MapTapSceneScene.cpp\n");
}

// on "init" you need to initialize your instance
bool MapTapScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	Director::getInstance()->setClearColor(Color4F(0.0f, 0.6f, 0.0f, 0.0f));

	// Adds a colour layer to background (default black)
	//LayerColor *_bgColor = LayerColor::create(Color4B(0, 191, 0, 255));
	//this->addChild(_bgColor, -1);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// Add Sprite
	m_pSprite = Sprite::create("face64x64.png");
	m_pSprite->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	//m_pSprite->setRotation(45.0f);
	m_pSprite->setScale(0.5f);
	m_pSprite->setCameraMask((unsigned short)CameraFlag::USER1);
	addChild(m_pSprite, 100);

	// create camera main
	initCamera();

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Register Touch Event
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	pListener = EventListenerTouchOneByOne::create();

	pListener->onTouchBegan = CC_CALLBACK_2(MapTapScene::onTouchBegan, this);
	pListener->onTouchMoved = CC_CALLBACK_2(MapTapScene::onTouchMoved, this);
	pListener->onTouchEnded = CC_CALLBACK_2(MapTapScene::onTouchEnded, this);

	dispatcher->addEventListenerWithSceneGraphPriority(pListener, this);

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"button-close64x64.png",
		"button-close-select64x64.png",
		CC_CALLBACK_1(MapTapScene::menuCloseCallback, this));

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
		CC_CALLBACK_1(MapTapScene::menuPlayCallback, this));

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

	auto label = Label::createWithTTF("MapTap v.1.0.1", "fonts/arial.ttf", 18);
	if (label == nullptr) {
		problemLoading("'fonts/arial.ttf'");
	}
	else {
		// position the label on the center of the screen
		label->setPosition(Vec2(origin.x + label->getContentSize().width,
			origin.y + visibleSize.height - label->getContentSize().height));

		label->setTextColor(Color4B(255, 255, 255, 255));
		// add the label as a child to this layer
		this->addChild(label, 2);
	}

	// draw vertical road
	//vector<Point> roadPoints = {
	//	{ 0, 300 },
	//	{ 0, -100 },

	//	{ 0, 0 },
	//	{ 200, 0 },

	//	{ 0, 75 },
	//	{ -100, 150 },
	//};

	//vector<Quad> roadQuadsLst = convertPointsToQuads(roadPoints);

	//for (const auto& q : roadQuadsLst) {
	//	CGameManager::cocos2dLog("roadQuadsLst : q.p[0] : ", q.p[0]);
	//	CGameManager::cocos2dLog("roadQuadsLst : q.p[1] : ", q.p[1]);
	//	CGameManager::cocos2dLog("roadQuadsLst : q.p[2] : ", q.p[2]);
	//	CGameManager::cocos2dLog("roadQuadsLst : q.p[3] : ", q.p[3]);

	//	Color4F roadColour(Color4F::RED);
	//	Color4F roadOutlineColour(Color4F(0.0f, 0.4f, 0.0f, 1.0f));
	//	auto poly = DrawNode::create();
	//	poly->drawPolygon(q.p, 4, roadColour, 1, roadOutlineColour);
	//	poly->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	//	this->addChild(poly, 11);
	//}

	//// draw vertical road
	//vector<Point> roadPoints = {
	//	{ 0, 300 },
	//	{ 0, -300 },

	//	{ 0, 0 },
	//	{ 200, 0 },

	//	{ 0, -70 },
	//	{ -200, -70 },

	//	{ 0, 75 },
	//	{ -100, 250 },

	//	{ 200, 300 },
	//	{ 200, -300 },
	//};

	/*Color4F roadColour(Color4F::BLACK);
	for (int n = 0; n < roadPoints.size(); n += 2) {
		m_pPolygon = DrawNode::create();
		m_pPolygon->setLineWidth(50.0f);
		m_pPolygon->drawLine(roadPoints[n], roadPoints[n + 1], roadColour);
		m_pPolygon->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
		m_pPolygon->setCameraMask((unsigned short)CameraFlag::USER1);
		this->addChild(m_pPolygon, 100);
	}*/

	// debug with lines (used by th cars)
	/*for (int n = 0; n < roadPoints.size(); n += 2) {
		m_pPolygon = DrawNode::create();
		m_pPolygon->setLineWidth(2.0f);
		m_pPolygon->drawLine(roadPoints[n], roadPoints[n + 1], Color4F::GRAY);
		m_pPolygon->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
		m_pPolygon->setCameraMask((unsigned short)CameraFlag::USER1);
		this->addChild(m_pPolygon, 100);
	}*/

	//auto m_pPolygonLine = DrawNode::create();
	//Vec2 centerPos = Vec2::ZERO;
	//constexpr int numPoints = 25;
	//Vec2 pointArr[numPoints];
	//for (int n = 0; n < numPoints; n++) {
	//	pointArr[n] = Vec2(centerPos.x, centerPos.y - 50.0f*n);
	//}
	//m_pPolygonLine->drawPoints(pointArr, numPoints, 10.0f, Color4F(1.0f, 1.0f, 1.0f, 0.9f));
	//m_pPolygonLine->setPosition(Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f));
	//m_pPolygonLine->setScaleX(0.1f);
	//this->addChild(m_pPolygonLine, 2);

	Vec2 CenterPoint = Vec2(visibleSize.width*0.5f, visibleSize.height*0.5f);
	float RotZero = -90.0f;

	//m_pLabel = Label::createWithTTF("piros utca", "fonts/arial.ttf", 16);
	//m_pLabel->setPosition(CenterPoint + Vec2(50.0f, 0.0f));
	//m_pLabel->setTextColor(Color4B(255, 255, 255, 255));
	//m_pLabel->setCameraMask((unsigned short)CameraFlag::USER1);
	//this->addChild(m_pLabel, 101);
	//TTFConfig configTest("fonts/PressStart2P.ttf", 32.0f);
	TTFConfig configTest("fonts/arial.ttf", 32.0f);
	Label* pLabelTest1 = Label::createWithTTF(configTest, "piros utca", cocos2d::TextHAlignment::CENTER);
	pLabelTest1->setTextColor(Color4B::WHITE);
	pLabelTest1->enableOutline(Color4B::BLACK, 2);
	Sprite* pNewSprite1 = createSpriteFromLabel(pLabelTest1);
	pNewSprite1->setPosition(CenterPoint + Vec2(50.0f, 0.0f));
	pNewSprite1->setRotation(0.0f);
	pNewSprite1->setScale(0.3f);
	pNewSprite1->setCameraMask((unsigned short)CameraFlag::USER1);
	//addChild(pNewSprite1, 200);

	// create sprite from this label test
	Label* pLabelTest = Label::createWithTTF(configTest, "where are we today", cocos2d::TextHAlignment::CENTER);
	pLabelTest->setTextColor(Color4B::WHITE);
	pLabelTest->enableOutline(Color4B::BLACK, 2);
	Sprite* pNewSprite = createSpriteFromLabel(pLabelTest);
	pNewSprite->setPosition(CenterPoint + Vec2(0.0f, 0.0f));
	pNewSprite->setRotation(90.0f);
	pNewSprite->setScale(0.3f);
	pNewSprite->setCameraMask((unsigned short)CameraFlag::USER1);
	//addChild(pNewSprite, 100);

	// create Snooker table 3D mesh
	m_pSprite3DCar = Sprite3D::create("plane.c3t");
	//m_pSprite3DCar->setTexture("green256x256.png");
	m_pSprite3DCar->setTexture("bodygreen64x64.png"); // 512x512
	m_pSprite3DCar->setContentSize(Size(32, 32)); // 2d plane
	m_pSprite3DCar->setScaleX(1.0f);
	m_pSprite3DCar->setScaleY(1.0f); // height of cube / car
	m_pSprite3DCar->setScaleZ(1.0f);
	m_pSprite3DCar->setPosition3D(Vec3(CenterPoint.x, CenterPoint.y, 0.0f));
	m_pSprite3DCar->setForce2DQueue(true);
	m_pSprite3DCar->setCameraMask((unsigned short)CameraFlag::USER1);
	//m_pSprite3DCar->setLightMask((unsigned int)cocos2d::LightFlag::LIGHT0);
	this->addChild(m_pSprite3DCar);

	// create Snooker table 3D mesh
	//auto m_pSprite3DRoad = Sprite3D::create("plane.c3t");
	//Size s = m_pSprite3DRoad->getContentSize();
	////Size scaleUnit = Size(s.width / 100.0f, s.height / 100.0f);
	//float scaleUnit = 1.0f / s.width;
	//cocos2d::log("s.width :%f, s.height : %f", s.width, s.height);
	//cocos2d::log("scaleUnit : %f", scaleUnit);

	//Point P1 = Point(0, 0);
	//Point P2 = Point(300, 300);

	//Vec2 M = P2 - P1;
	//float rot = CC_RADIANS_TO_DEGREES(M.getAngle());
	//float mag = M.length();
	//cocos2d::log("rot: %f", rot);
	//cocos2d::log("mag: %f", mag);
	//M = P1 + M * 0.5f;
	//cocos2d::log("P1.x: %f, P1.y: %f", P1.x, P1.y);
	//cocos2d::log("P2.x: %f, P2.y: %f", P2.x, P2.y);
	//cocos2d::log("M.x: %f, M.y: %f", M.x, M.y);

	//float roadWidth = scaleUnit * 20.0f;
	//float roadHeight = scaleUnit * 2.0f;
	//float roadLength = scaleUnit * mag;

	//m_pSprite3DRoad->setColor(Color3B(0, 0, 0));
	//m_pSprite3DRoad->setContentSize(Size(128, 128)); // 2d plane
	//m_pSprite3DRoad->setScaleX(roadWidth);
	//m_pSprite3DRoad->setScaleY(roadLength);
	//m_pSprite3DRoad->setScaleZ(roadHeight);
	//m_pSprite3DRoad->setPosition3D(Vec3(CenterPoint.x + M.x, CenterPoint.y + M.y, 3.0f));
	//m_pSprite3DRoad->setRotation3D(Vec3(0, 0, rot));
	//m_pSprite3DRoad->setForce2DQueue(true);
	//m_pSprite3DRoad->setCameraMask((unsigned short)CameraFlag::USER1);
	//this->addChild(m_pSprite3DRoad);

	m_pCar = Car::create("box.c3t", Vec3(CenterPoint.x, CenterPoint.y, 10.0f));
	this->addChild(m_pCar->pSprite3D);

	// draw vertical road
	vector<Point> roadPoints = {
		{0,0},
		{250,250},
		{ 0,0 },
		{ 0,250 },
		{ 0,0 },
		{ 250,0 },

		{ 0, 300 },
		{ 0, -300 },

		{ 0, 0 },
		{ 200, 0 },

		{ 0, -70 },
		{ -200, -70 },

		{ 0, 75 },
		{ -100, 250 },

		{ 200, 300 },
		{ 200, -300 },
	};

	for (int n = 0; n < roadPoints.size(); n += 2) {
		auto pSprite3DRoad = Sprite3D::create("plane.c3t");

		Size s = pSprite3DRoad->getContentSize();
		float scaleUnit = 1.0f / s.width;
		Point P1 = roadPoints[n];
		Point P2 = roadPoints[n + 1];
		Vec2 M = P2 - P1;
		float rot = CC_RADIANS_TO_DEGREES(M.getAngle());
		float mag = M.length();

		M = P1 + M * 0.5f;

		float roadWidth = scaleUnit * 20.0f;
		float roadHeight = scaleUnit * 2.0f;
		float roadLength = scaleUnit * mag;

		pSprite3DRoad->setColor(Color3B(35, 35, 35));
		pSprite3DRoad->setTexture("background_cave.jpg"); // 512x512
		pSprite3DRoad->setContentSize(Size(128, 128)); // 2d plane
		pSprite3DRoad->setScaleX(roadWidth);
		pSprite3DRoad->setScaleY(roadLength);
		pSprite3DRoad->setScaleZ(roadHeight);
		pSprite3DRoad->setPosition3D(Vec3(CenterPoint.x + M.x, CenterPoint.y + M.y, 3.0f));
		pSprite3DRoad->setRotation3D(Vec3(0, 0, RotZero - rot));
		pSprite3DRoad->setForce2DQueue(true);
		pSprite3DRoad->setCameraMask((unsigned short)CameraFlag::USER1);
		this->addChild(pSprite3DRoad);
		m_pSpriteRoad = pSprite3DRoad;
	}

	// create road name
	auto pSprite3DRoadName = Sprite3D::create("plane.c3t");

	Size s = pSprite3DRoadName->getContentSize();
	float scaleUnit = 1.0f / s.width;
	Point P1 = roadPoints[0];
	Point P2 = roadPoints[0 + 1];
	Vec2 M = P2 - P1;
	float rot = CC_RADIANS_TO_DEGREES(M.getAngle());
	float mag = M.length();

	M = P1 + M * 0.5f;

	float roadWidth = scaleUnit * pNewSprite1->getContentSize().width * 0.3f;
	float roadHeight = scaleUnit * 2.0f;
	float roadLength = scaleUnit * pNewSprite1->getContentSize().height * 0.3f;

	pSprite3DRoadName->setTexture(pNewSprite1->getTexture());
	pSprite3DRoadName->setContentSize(Size(128, 128)); // 2d plane
	pSprite3DRoadName->setScaleX(roadWidth);
	pSprite3DRoadName->setScaleY(roadLength);
	pSprite3DRoadName->setScaleZ(0.0f);
	pSprite3DRoadName->setPosition3D(Vec3(CenterPoint.x + M.x, CenterPoint.y + M.y, 4.0f));
	pSprite3DRoadName->setRotation3D(Vec3(0, 0, -rot));
	pSprite3DRoadName->setForce2DQueue(true);
	pSprite3DRoadName->setCameraMask((unsigned short)CameraFlag::USER1);
	this->addChild(pSprite3DRoadName);

	// create road name 2
	auto pSprite3DRoadName2 = Sprite3D::create("plane.c3t");
	{
		Size s = pSprite3DRoadName2->getContentSize();
		float scaleUnit = 1.0f / s.width;
		Point P1 = roadPoints[4];
		Point P2 = roadPoints[4 + 1];
		Vec2 M = P2 - P1;
		float rot = CC_RADIANS_TO_DEGREES(M.getAngle());
		float mag = M.length();

		M = P1 + M * 0.5f;

		float roadWidth = scaleUnit * pNewSprite->getContentSize().width * 0.3f;
		float roadHeight = scaleUnit * 2.0f;
		float roadLength = scaleUnit * pNewSprite->getContentSize().height * 0.3f;

		pSprite3DRoadName2->setTexture(pNewSprite->getTexture());
		pSprite3DRoadName2->setContentSize(Size(128, 128)); // 2d plane
		pSprite3DRoadName2->setScaleX(roadWidth);
		pSprite3DRoadName2->setScaleY(roadLength);
		pSprite3DRoadName2->setScaleZ(0.0f);
		pSprite3DRoadName2->setPosition3D(Vec3(CenterPoint.x + M.x, CenterPoint.y + M.y, 4.0f));
		pSprite3DRoadName2->setRotation3D(Vec3(0, 0, -rot));
		pSprite3DRoadName2->setForce2DQueue(true);
		pSprite3DRoadName2->setCameraMask((unsigned short)CameraFlag::USER1);
		this->addChild(pSprite3DRoadName2);
	}

	// TODO: Camera should track the 3d car on the map, untiil  it crashes and explods into parts scattered around!

	// start update loop
	scheduleUpdate();

	return true;
}

void MapTapScene::update(float dt)
{
	//cocos2d::log("update...");
	//this->setScale(2.0f);
	// TODO: get the Camera 3D working for handling zoom in-out affects!

	if (m_pCar) {
		m_pCar->process();
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec3 P = m_pCamera->getPosition3D();
	Vec3 CarPos = m_pCar->pSprite3D->getPosition3D() + Vec3(0, visibleSize.height*0.1f, 0);
	m_pCamera->setPosition3D(CarPos + Vec3(m_R.x * 210.0f, m_R.y * 210.0f, m_CameraPos.z));
	m_pCamera->lookAt(CarPos, Vec3::UNIT_Z);

	//if (m_pSpriteRoad) {
	//	m_pSpriteRoad->setRotation3D(Vec3(0, 0, (int(m_seq) % 360)));
	//}

	m_seq++;
	m_cyc++;
}

void MapTapScene::menuCloseCallback(Ref* pSender)
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

void MapTapScene::menuPlayCallback(Ref* pSender)
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect("click.wav");
	// replace scene
	CGameManager::Instance()->m_stringGameType = "Game";
	CGameManager::Instance()->RunScene(kViewMapTap);
}

bool MapTapScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("MapTapScene::onTouchBegan : You touched %f, %f", location.x, location.y);

	// get camera
	auto camera = Camera::getVisitingCamera();
	if (!camera)
	{
		cocos2d::log("FAILED NO CAMERA!!!");
		return false;
	}

	return true;
}

void MapTapScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	// get camera
	auto camera = Camera::getVisitingCamera();
	if (!camera)
	{
		cocos2d::log("FAILED NO CAMERA!!!");
		return;
	}

	m_bDragWorld = true;
	if (m_bDragWorld) {
		// debug - touch deltas turn table effect to see whole 3D world
		Vec2 delta = touch->getDelta();
		//m_angle -= CC_DEGREES_TO_RADIANS(delta.x * 2.0f * 0.2f);
		//if (m_CameraPos.z < 0.1f) {
		//	m_CameraPos.z = 0.1f;
		//}
		//float fx = cos(-m_angle) * m_CameraPos.z;
		//float fz = sin(-m_angle) * m_CameraPos.z;
		//m_pCamera->setPosition3D(m_CameraTarget + Vec3(fx, m_CameraPos.y, fz));
		//m_pCamera->lookAt(m_CameraTarget, Vec3(0.0f, 1.0f, 0.0f));
		//updateCamera(m_CameraTarget, m_angle);

		m_R.x = -sin(CC_DEGREES_TO_RADIANS(m_angle));
		m_R.y = cos(CC_DEGREES_TO_RADIANS(m_angle));

		float yMag = fabs(delta.y);
		float xMag = fabs(delta.x);

		if (yMag > 4.0f && yMag > xMag) {
			m_CameraTarget.x += m_R.x * delta.y * 1.0f;
			m_CameraTarget.y += m_R.y * delta.y * 1.0f;
		}
		else if (xMag > 0.5f) {
			m_angle += delta.x * 0.2f;
		}

		//m_pCamera->setRotation(m_angle);

		Vec3 P = m_pCamera->getPosition3D();
		CGameManager::cocos2dLog("P : ", P);

		//fx = sin(CC_DEGREES_TO_RADIANS(180.0f - 45.0f));
		//fy = cos(CC_DEGREES_TO_RADIANS(180.0f - 45.0f));

		//		P.x -= fx * delta.y * 1.0f;
		//		P.y -= fy * delta.y * 1.0f;

				//m_CameraPos.x += delta.y * 0.2f;
				//m_pCamera->setPosition3D(m_CameraPos);
		//		m_pCamera->setPosition3D(P);

		//m_pCamera->setPosition3D(m_CameraTarget + Vec3(m_R.x * 210.0f, m_R.y * 210.0f, m_CameraPos.z));
		//m_pCamera->lookAt(m_CameraTarget, Vec3::UNIT_Z);
		//visibleSize.height*0.1f

		Size visibleSize = Director::getInstance()->getVisibleSize();
		Vec3 CarPos = m_pCar->pSprite3D->getPosition3D() + Vec3(0, visibleSize.height*0.1f, 0);
		m_pCamera->setPosition3D(CarPos + Vec3(m_R.x * 210.0f, m_R.y * 210.0f, m_CameraPos.z));
		m_pCamera->lookAt(CarPos, Vec3::UNIT_Z);
		// TODO: fix camera angle so car always stays at bottom of screen

		cocos2d::log("m_angle: %f", m_angle);
		if (m_angle < 90.0f && m_angle >= -90.0f) {
			m_pCar->Vel.z = -2.0f;
		}
		else {
			m_pCar->Vel.z = 2.0f;
		}
	}
}

void MapTapScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	Point location = touch->getLocationInView();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	location.y = visibleSize.height - location.y; // invert direction
	cocos2d::log("MapTapScene::onTouchEnded : You touched %f, %f", location.x, location.y);

	// get camera
	auto camera = Camera::getVisitingCamera();
	if (!camera)
	{
		cocos2d::log("FAILED NO CAMERA!!!");
		return;
	}

	m_bDragWorld = false;
}

vector<Quad> MapTapScene::convertPointsToQuads(const vector<Point>& roadPoints) {
	vector<Quad> quads;
	vector<Quad> newQuads;
	float padding = 20.0f;

	// Step #1 - create sqaures for points
	for (auto& p : roadPoints) {
		Quad q = { Point::ZERO, Point::ZERO, Point::ZERO, Point::ZERO };
		q.origin.x = p.x;
		q.origin.y = p.y;

		q.p[0].x = p.x - padding;
		q.p[0].y = p.y - padding;

		q.p[1].x = p.x - padding;
		q.p[1].y = p.y + padding;

		q.p[2].x = p.x + padding;
		q.p[2].y = p.y + padding;

		q.p[3].x = p.x + padding;
		q.p[3].y = p.y - padding;

		quads.push_back(q);
	}

	// Step #2 - create paths from the squares (2 consecutive quads from list above)
	for (int n = 0; n < quads.size(); n += 2) {
		Quad newQuad = calcQuadFromPair(quads[n], quads[n + 1]);
		newQuads.push_back(newQuad);
	}

	return newQuads;
}

Quad MapTapScene::calcQuadFromPair(Quad& q1, Quad& q2) {
	Quad q = { Point::ZERO, Point::ZERO, Point::ZERO, Point::ZERO };
	Vec2 V1;
	Vec2 V2 = Vec2(0.0f, 1.0f);
	V1 = q2.origin - q1.origin;
	float ang = V1.angle(V2, V1);
	cocos2d::log("ang : %f", ang);

	float minX = compareValues(q1.getMinX(), q2.getMinX(), [](float a, float b) { return a < b; });
	float minY = compareValues(q1.getMinY(), q2.getMinY(), [](float a, float b) { return a < b; });
	float maxX = compareValues(q1.getMaxX(), q2.getMaxX(), [](float a, float b) { return a > b; });
	float maxY = compareValues(q1.getMaxY(), q2.getMaxY(), [](float a, float b) { return a > b; });

	q.p[0] = Point(minX, minY);
	q.p[1] = Point(minX, maxY);
	q.p[2] = Point(maxX, maxY);
	q.p[3] = Point(maxX, minY);

	cocos2d::log("minX : %f", minX);
	cocos2d::log("minY : %f", minY);
	cocos2d::log("maxX : %f", maxX);
	cocos2d::log("maxY : %f", maxY);

	return q;
}

float Quad::getMinX() {
	float minX = numeric_limits<int>::max();
	for (int n = 0; n < 4; n++) {
		if (this->p[n].x < minX) {
			minX = this->p[n].x;
		}
	}
	return minX;
}

float Quad::getMinY() {
	float minY = numeric_limits<int>::max();
	for (int n = 0; n < 4; n++) {
		if (this->p[n].y < minY) {
			minY = this->p[n].y;
		}
	}
	return minY;
}

float Quad::getMaxX() {
	float maxX = numeric_limits<int>::min();
	for (int n = 0; n < 4; n++) {
		if (this->p[n].x > maxX) {
			maxX = this->p[n].x;
		}
	}
	return maxX;
}

float Quad::getMaxY() {
	float maxY = numeric_limits<int>::min();
	for (int n = 0; n < 4; n++) {
		if (this->p[n].y > maxY) {
			maxY = this->p[n].y;
		}
	}
	return maxY;
}

float MapTapScene::compareValues(float val1, float val2, std::function<float(float, float)> fn) {
	return fn(val1, val2) ? val1 : val2;
}

void MapTapScene::initCamera(void) {
	Size size = Director::getInstance()->getWinSize();
	//auto size = Director::getInstance()->getVisibleSize();
	float zeye = Director::getInstance()->getZEye();

	//m_angle = CC_DEGREES_TO_RADIANS(90.0f);// 0.0f
	m_angle = 180.0f;
	//m_CameraPos = Vec3(size.width*0.5f, size.height*0.5f, 200.0f);
	//m_CameraPos = Vec3(0.0f, 0.0f, 100.0f);
	// create camera
	//m_CameraPos = Vec3(0.0f, 150.0f, 250.0f);
	//m_CameraPos = Vec3(0.0f, 250.0f, 400.0f);
	//m_CameraPos = Vec3(0.0f, 100.0f, 550.0f);
	//m_CameraTarget = m_pSprite3DBall->getPosition3D();

	//m_CameraTarget = Vec3(size.width*0.5f, size.height*0.5f, 0.0f);
	//m_CameraTarget = Vec3(0, 0, 0.0f);
	//m_CameraTarget = m_pSprite3DOrc->getPosition3D();

	//m_CameraPos = Vec3(m_CameraTarget.x, m_CameraTarget.y, 200.0f);
	m_CameraPos = Vec3(m_CameraTarget.x, m_CameraTarget.y, 150.0f);
	m_CameraTarget = m_pSprite->getPosition3D();

	m_CameraRot = Vec3::ZERO;
	//m_pCamera = Camera::createPerspective(30, (GLfloat)200.0f / 200.0f, 10, 2000.0f); // 1:1 aspect ratio for camera 3D view
	m_pCamera = Camera::createPerspective(30, (GLfloat)size.width / size.height, 10, 2000.0f); // 1:1 aspect ratio for camera 3D view
	//m_pCamera = Camera::createOrthographic(size.width, size.height, 1, 1000);
	cocos2d::log("Camera : size.width : %f, size.height : %f", size.width, size.height);
	//m_pCamera = Camera::createPerspective(60, (GLfloat)size.width / size.height, 10, zeye + size.height / 2.0f);
	//m_pCamera->setPosition3D(m_CameraPos);

	//++m_pCamera = this->getDefaultCamera();

	m_pCamera->setPosition3D(m_CameraTarget + Vec3(0.0f * 210.0f, -1.0f * 210.0f, m_CameraPos.z));
	m_pCamera->lookAt(m_CameraTarget, Vec3::UNIT_Z);

	float farPlane = m_pCamera->getFarPlane();
	float nearPlane = m_pCamera->getNearPlane();
	cocos2d::log("nearPlane : %f, farPlane : %f", nearPlane, farPlane);
	//m_pCamera->setPosition3D(Vec3(0, 0, 100));
//	m_pCamera->lookAt(Vec3(size.width*0.5f, size.height*0.5f, 100.0f), Vec3(0, 0, 1));

	//m_pCamera->setRotation(45.0f);

	////m_sizeRT.width
	//m_pCamera->setPosition3D(m_CameraPos);
	//m_pCamera->lookAt(m_CameraTarget, Vec3::UNIT_Y);

	//// ------------- //
	//// touch deltas turn table effect to see whole 3D world (snippet taken from touch move code)
	////float fx = sin(m_angle*0.2f) * m_CameraPos.z;
	////float fz = cos(m_angle*0.2f) * m_CameraPos.z;
	//float fx = cos(-m_angle) * m_CameraPos.z;
	//float fz = sin(-m_angle) * m_CameraPos.z;
	////m_pCamera->setPosition3D(Vec3(fx, m_CameraPos.y, fz));
	//m_pCamera->setPosition3D(m_CameraTarget + Vec3(fx, m_CameraPos.y, fz));
	////m_pCamera->lookAt(Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	////m_pCamera->lookAt(Vec3(0.0f, m_pSprite3DBall->getContentSize().height*0.5f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));
	//m_pCamera->lookAt(m_CameraTarget, Vec3(0.0f, 1.0f, 0.0f));

	m_pCamera->setCameraFlag(CameraFlag::USER1);
	this->addChild(m_pCamera);

	// set camera mask of this scene
	this->setCameraMask((unsigned short)CameraFlag::USER1);
	////this->setCameraMask((unsigned short)CameraFlag::DEFAULT);
}

void MapTapScene::updateCamera(Vec3 pos, float angle) {
	// if too close, then move back to old pos (undo)
	if (m_CameraPos.z < 0.1f) {
		m_CameraPos.z = 0.1f;
	}

	float fx = cos(-m_angle*0.2f) * m_CameraPos.z;
	float fz = sin(-m_angle*0.2f) * m_CameraPos.z;

	m_pCamera->setPosition3D(m_CameraTarget + Vec3(fx, m_CameraPos.y, fz));
	m_pCamera->lookAt(m_CameraTarget, Vec3(0.0f, 1.0f, 0.0f));
}

Sprite* MapTapScene::createSpriteFromLabel(Label* pLabel) {
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