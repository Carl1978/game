//
//  Floor.cpp
//  game
//
//  Created by Carl Phillips on 21/10/2018.
//
//

#include "Floor.h"

USING_NS_CC;

int Floor::total = 0;

Floor::Floor(Scene *parent) : Entity() {
    pParent = parent;
}

Floor::~Floor()
{
}

void Floor::init(void) {
    
}

void Floor::spawn(const std::string filename, Vec2 P) {
	type = "floor";
	pos = P;
	pSprite = Sprite::create(filename);
	pSprite->setPosition(P);
	pParent->addChild(pSprite, 1);
	Floor::total++;
}

void Floor::deinit(void) {
    
}

void Floor::process(void) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float width = pSprite->getContentSize().width;

	pos.x-=3;

	if (pos.x < -width) {
		pos.x += Floor::total * pSprite->getContentSize().width;
	}

	pSprite->setPosition(pos);
	cyc++;
	seq++;
}

void Floor::draw(void) {
    
}
