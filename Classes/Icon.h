//
//  Icon.h
//  game
//
//  Created by Carl Phillips on 9/1/2019.
//
//

#ifndef __game__Icon__
#define __game__Icon__

#include <iostream>
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Icon : public Entity {
public:
	std::string value;
	Icon();
	~Icon();
	void init(void);
	Sprite* spawn(const std::string filename = "HelloWorld.png", Vec2 P = Vec2::ZERO);
	Sprite* spawn(const std::string& val, Sprite* pSpr, Vec2 P = Vec2::ZERO);
	static Icon* create(const std::string filename = "HelloWorld.png", Vec2 P = Vec2::ZERO);
	static Icon* create(const std::string& val, Sprite* pSpr, Vec2 P = Vec2::ZERO);
	void deinit(void);
	void process(void);
	void draw(void);
};

#endif /* defined(__game__Icon__) */
