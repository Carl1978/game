#ifndef __TEMPLATE_SCENE_H__
#define __TEMPLATE_SCENE_H__

#include "cocos2d.h"
#include "json/document.h"
#include "Entity.h"

USING_NS_CC;
using namespace rapidjson;

class TemplateScene : public cocos2d::Scene
{
public:
	TemplateScene();
	~TemplateScene();
	static cocos2d::Scene* createScene();

	EventListenerTouchOneByOne* pListener;

	float m_seq;
	long m_cyc;

	Label* m_pLabel;
	Entity* m_pEntity;
	Sprite* m_pSprite;

	virtual bool init();
	void update(float dt);

	// a selector callback
	void menuCloseCallback(Ref* pSender);
	void menuPlayCallback(Ref* pSender);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	// implement the "static create()" method manually
	CREATE_FUNC(TemplateScene);
};

#endif // __TEMPLATE_SCENE_H__
