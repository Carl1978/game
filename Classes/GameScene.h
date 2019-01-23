#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "json/document.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Letter.h"
#include "Floor.h"
#include "Boat.h"
#include "Icon.h"
#include "IconString.h"

USING_NS_CC;
using namespace rapidjson;

class GameScene : public cocos2d::Scene
{
public:
	GameScene();
	~GameScene();
	static cocos2d::Scene* createScene();

	rapidjson::Document m_documentLetters;
	rapidjson::Document m_document;

	Entity* pEntity;
	EntityManager* pEntityManager;
	Letter* pLetter;
	std::vector<Floor*> pFloorList;

	EventListenerTouchOneByOne* pListener;
	Sprite* m_pSpritePlayer;
	Sprite* m_pSpriteLazer;
	Sprite* m_pSpriteFloor;
	Sprite* m_pSpriteMarker;
	Sequence* seqTint;
	Sequence* seqShake;
	Sequence* seqGlow;
	Sequence* seqScale;
	bool bShake;
	Vector<SpriteFrame*> animFrames;
	Animation* animation;
	Animate* animate;
	TintTo* tint;
	RotateTo* rotateToLeft;
	RotateTo* rotateToRight;
	Sequence* seqJelly;
	RotateTo* rotateToShoot;
	RepeatForever* repeatSeqJelly;
	Sprite* waveSpr[4][6];
	Vec2 wavePos[4];
	float m_seq;
	long m_cyc;
	Sprite* m_pSpriteBoat;
	Sprite* m_pSpriteMan;
	DrawNode* m_pPolygon;

	Boat* m_pBoat;
	std::shared_ptr<Boat> m_boat;
	std::shared_ptr<Boat> m_boatArr[4];
	std::shared_ptr<Boat>* m_pBoatArr;

	Icon* m_pIconB;
	std::shared_ptr<Icon> m_iconArr[7];

	std::shared_ptr<IconString> m_iconStringBonjour;

	std::vector<Sprite*> m_spriteLetters;
	std::vector<Icon*> m_iconLetters;

	virtual bool init();
	void update(float dt);
	void shakeTheWorld(bool bStatus = true);
	Sprite* createSpriteFromLabel(Label* pLabel);
	std::vector<Sprite*> createSpriteArrFromLabel(Label* pLabel, const std::string& text);
	std::vector<Sprite*> createSpriteArrFromString(const std::string& text);
	std::vector<Icon*> createIconArrFromString(const std::string& text);
	rapidjson::Document parseJSON(const std::string& filename);
	int getIdxFromIconValue(std::vector<Icon*> iconLetters, const std::string value);

	// a selector callback
	void menuCloseCallback(Ref* pSender);
	void menuPlayCallback(Ref* pSender);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
