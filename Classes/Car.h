//
//  Car.h
//  game
//
//  Created by Carl Phillips on 8/1/2019.
//
//

#ifndef __game__Car__
#define __game__Car__

#include <iostream>
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Car : public Entity {
public:
	Vec3 startPos;
	Vec3 Pos;
	Vec3 Vel;
	Sprite3D *pSprite3D;

	Car();
	~Car();
	void init(void);
	Sprite3D* spawn(const std::string filename = "HelloWorld.png", Vec3 P = Vec3::ZERO);
	static Car* create(const std::string filename = "HelloWorld.png", Vec3 P = Vec3::ZERO);
	void deinit(void);
	void process(void);
	void draw(void);
};

#endif /* defined(__game__Car__) */
