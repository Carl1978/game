#ifndef __SNAKE_SCENE_H__
#define __SNAKE_SCENE_H__

#include "cocos2d.h"
#include "Entity.h"
#include "EntityManager.h"
#include "Letter.h"
#include "Floor.h"

USING_NS_CC;

class SnakeScene : public cocos2d::Scene
{
public:
	SnakeScene();
	~SnakeScene();
    static cocos2d::Scene* createScene();

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

    virtual bool init();
	void update(float dt);
	void shakeTheWorld(bool bStatus = true);

    // a selector callback
    void menuCloseCallback(Ref* pSender);
	void menuPlayCallback(Ref* pSender);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    // implement the "static create()" method manually
    CREATE_FUNC(SnakeScene);
};

#endif // __SNAKE_SCENE_H__
