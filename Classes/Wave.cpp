//
//  Wave.cpp
//  game
//
//  Created by Carl Phillips on 02/12/2019.
//
//

#include "Wave.h"

USING_NS_CC;

int Wave::waveSegmentCount = 6;

Wave::Wave() : Entity() {
	//cocos2d::log("Wave ctr...");
	idx = 0;
}

Wave::~Wave()
{
	//cocos2d::log("Wave dtr...");
}

Sprite* Wave::spawn(const std::string filename, Vec2 P, int waveIdx) {
	Entity::spawn(P);

	pSprite = Sprite::create(filename);
	idx = waveIdx;
	float maxSpeed = 4.0f;//6.0f;//4.0f;
	vel = Vec2(maxSpeed - idx, 0.0f);
	cocos2d::log("wave::spawn :: idx : %d", idx);
	cocos2d::log("Wave::spawn :: vel.x : %f", vel.x);
	pos = startPos = P;
	pSprite->setPosition(P);
	type = "Wave";
	cocos2d::log("Wave::spawn()...");
	return pSprite;
}

Wave* Wave::create(const std::string filename, Vec2 P, int waveIdx) {
	Wave* pWave = new Wave();
	pWave->spawn(filename, P, waveIdx);
	return pWave;
}

void Wave::init(void) {
	// TODO:
}

void Wave::deinit(void) {
	// TODO:
}

void Wave::process(void) {
	float dist = startPos.x - pos.x;
	if (dist >= 256.0f) {
		pos = startPos;
	}
	else {
		pos.x -= vel.x;
	}

	pos.y = startPos.y + cos((seq + idx * 20.0f) * 0.05f) * 4.0f;

	pSprite->setPosition(pos);

	cyc++;
	seq++;
}

void Wave::draw(void) {
	// TODO:
}