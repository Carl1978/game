//
//  Boat.h
//  game
//
//  Created by Carl Phillips on 8/1/2019.
//
//

#ifndef __game__Boat__
#define __game__Boat__

#include <iostream>
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Boat : public Entity {
public:
	Boat();
	~Boat();
	void init(void);
	Sprite* spawn(const std::string filename = "HelloWorld.png", Vec2 P = Vec2::ZERO);
	static Boat* create(const std::string filename = "HelloWorld.png", Vec2 P = Vec2::ZERO);
	void deinit(void);
	void process(void);
	void draw(void);
};

#endif /* defined(__game__Boat__) */
