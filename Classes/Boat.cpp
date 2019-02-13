//
//  Boat.cpp
//  game
//
//  Created by Carl Phillips on 21/10/2018.
//
//

#include "Boat.h"

USING_NS_CC;

Boat::Boat() : Entity() {
	//cocos2d::log("Boat ctr...");
}

Boat::~Boat()
{
	//cocos2d::log("Boat dtr...");
}

Sprite* Boat::spawn(const std::string filename, Vec2 P) {
	Entity::spawn(P);

	pSprite = Sprite::create(filename);
	pos = startPos = P;
	pSprite->setPosition(P);
	type = "Boat";
	cocos2d::log("Boat::spawn()...");
	return pSprite;
}

Boat* Boat::create(const std::string filename, Vec2 P) {
	Boat* pBoat = new Boat();
	pBoat->spawn(filename, P);
	return pBoat;
}

void Boat::init(void) {
	// TODO:
}

void Boat::deinit(void) {
	// TODO:
}

void Boat::process(void) {
	float dy = cos((seq + 40.0f) * 0.1f) * 6.0f;
	float dr = cos((seq + 20.0f) * 0.1f) * 8.0f;

	pos = startPos;
	pos.y += dy;
	pSprite->setPosition(pos);
	pSprite->setRotation(dr);

	cyc++;
	seq++;
}

void Boat::draw(void) {
	// TODO:
}