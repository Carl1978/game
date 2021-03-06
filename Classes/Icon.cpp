//
//  Icon.cpp
//  game
//
//  Created by Carl Phillips on 9/1/2019.
//
//

#include "Icon.h"

USING_NS_CC;

Icon::Icon() : Entity() {
	// TODO:
//	cocos2d::log("Icon ctr...");
	// TOOD: use id from Entity
}

Icon::~Icon()
{
	// TODO:
	cocos2d::log("Icon dtr...");
}

Sprite* Icon::spawn(const std::string filename, Vec2 P) {
	Entity::spawn(P);

	pSprite = Sprite::create(filename);
	pos = startPos = P;
	pSprite->setPosition(P);
	type = "Icon";
	value = "";
	//	cocos2d::log("Icon::spawn()...");
	return pSprite;
}

Sprite* Icon::spawn(const std::string& val, Sprite* pSpr, Vec2 P) {
	Entity::spawn(P);

	pos = startPos = P;
	pSprite = pSpr;
	if (pSprite != nullptr) {
		pSprite->setPosition(P);
	}
	type = "Icon";
	value = val;
	//	cocos2d::log("Icon::spawn()...");

	return pSprite;
}

Icon* Icon::create(const std::string filename, Vec2 P) {
	Icon* pIcon = new Icon();
	pIcon->spawn(filename, P);
	return pIcon;
}

Icon* Icon::create(const std::string& val, Sprite* pSpr, Vec2 P) {
	Icon* pIcon = new Icon();
	pIcon->spawn(val, pSpr, P);
	return pIcon;
}

void Icon::init(void) {
	// TODO:
}

void Icon::deinit(void) {
	// TODO:
}

void Icon::process(void) {
	// TODO:
}

void Icon::draw(void) {
	// TODO:
}