//
//  Wave.h
//  game
//
//  Created by Carl Phillips on 02/12/2019.
//
//

#ifndef __game__Wave__
#define __game__Wave__

#include <iostream>
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Wave : public Entity {
public:
	static int waveSegmentCount;
	int idx;
	Vec2 startPos;

	Wave();
	~Wave();
	void init(void);
	Sprite* spawn(const std::string filename = "HelloWorld.png", Vec2 P = Vec2::ZERO, int waveIdx = 0);
	static Wave* create(const std::string filename = "HelloWorld.png", Vec2 P = Vec2::ZERO, int waveIdx = 0);
	void deinit(void);
	void process(void);
	void draw(void);
};

#endif /* defined(__game__Wave__) */
