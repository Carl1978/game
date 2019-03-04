#ifndef __MAP_TAP_SCENE_H__
#define __MAP_TAP_SCENE_H__

#include "cocos2d.h"
#include "json/document.h"
#include "Entity.h"
#include "Car.h"

USING_NS_CC;
using namespace std;
using namespace rapidjson;

struct Quad {
	Point origin;
	Point p[4];
	float getMinX();
	float getMinY();
	float getMaxX();
	float getMaxY();
};

class MapTapScene : public cocos2d::Scene
{
public:
	MapTapScene();
	~MapTapScene();
	static cocos2d::Scene* createScene();

	EventListenerTouchOneByOne* pListener;

	float m_seq;
	long m_cyc;

	Camera *m_pCamera;
	Camera *m_pCameraStats;
	Vec3 m_CameraTarget;
	Vec3 m_CameraPos;
	Vec3 m_CameraRot;
	float m_angle;
	float m_dist;
	Vec2 m_delta;
	Vec2 m_deltaTotal;
	bool m_bDragWorld;
	Vec2 m_R;

	Label* m_pLabel;
	Entity* m_pEntity;
	Sprite* m_pSprite;
	DrawNode* m_pPolygon;
	Sprite3D *m_pSprite3DCar;
	Sprite3D *m_pSpriteRoad;

	Car* m_pCar;

	virtual bool init();
	void update(float dt);
	vector<Quad> convertPointsToQuads(const vector<Point>& roadPoints);
	Quad calcQuadFromPair(Quad& q1, Quad& q2);
	float compareValues(float val1, float val2, std::function<float(float, float)> fn);
	void initCamera(void);
	void updateCamera(Vec3 pos, float angle);
	Sprite* createSpriteFromLabel(Label* pLabel);

	// a selector callback
	void menuCloseCallback(Ref* pSender);
	void menuPlayCallback(Ref* pSender);
	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	// implement the "static create()" method manually
	CREATE_FUNC(MapTapScene);
};

#endif // __MAP_TAP_SCENE_H__
