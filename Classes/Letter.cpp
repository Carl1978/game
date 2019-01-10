//
//  Letter.cpp
//  game
//
//  Created by Carl Phillips on 21/10/2018.
//
//

#include "Letter.h"

USING_NS_CC;

Vec2 Letter::T = Vec2(0, 0);

Letter::Letter(Scene *parent) : Entity() {
    pParent = parent;
	letter = "";
	state = 0;
	status = 0;
	belt = 1;
}

Letter::~Letter()
{
}

void Letter::init(void) {
    
}

void Letter::spawn(const std::string letter, int belt, const std::string filename, Vec2 P) {
	status = 1;
	state = eState::STATE_LETTER_ALIVE;
	type = "letter";
	this->belt = belt;
	this->letter = letter;//std::string(letter.c_str());

	std::string fname = filename;
	if (letter == "B" || letter == "o" || letter == "n" || letter == "j" || letter == "u" || letter == "r") {
		// override the filename if this is a letter sprite!
		std::string newFilename = this->letter.c_str();
		newFilename += ".png";
		cocos2d::log("newFilename: %s", newFilename.c_str());
		fname = newFilename;
	}
	cocos2d::log("fname: %s", fname.c_str());

	startPos = pos = P;
	pSprite = Sprite::create(fname);
	pSprite->setPosition(P);
	pParent->addChild(pSprite, 20);

	// test - make sure we can add label as a child too sprite
	pLabel = Label::createWithTTF(letter, "fonts/arial.ttf", 24);
	pLabel->setPosition(Vec2(0, 0));
	pLabel->setTextColor(Color4B(0, 0, 0, 255));
	//pLabel->setAlignment(TextHAlignment::CENTER, ::TextVAlignment::CENTER);
	//pLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	pSprite->addChild(pLabel, 1);

	float w = fabs(pLabel->getBoundingBox().getMaxX() - pLabel->getBoundingBox().getMinX() );
	float h = fabs(pLabel->getBoundingBox().getMaxY() - pLabel->getBoundingBox().getMinY());
	cocos2d::log("letter \'%s\': %f, %f", this->letter.c_str(), w, h);
	size = Size(w, h);
	//label->setPosition(Vec2(w*0.5f, h*0.5f));
	//float sx = w * (1.0f / 8.0f);
	//float sy = h * (1.0f / 8.0f);
	//pSprite->setScale(sx, sy);
	//pSprite->setOpacity(0); // hide the sprite (white box for now)
	//pSprite->setCascadeOpacityEnabled(true); // affects all the children as well
	pLabel->setVisible(false);
	pSprite->setScale(0.4f); // 0.5f
	pSprite->setColor(Color3B(0, 0, 0));
}

void Letter::deinit(void) {
    
}

void Letter::process(void) {
	//pSprite->setRotation(float(cyc % 360));

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float width = pSprite->getContentSize().width;
	float offset = belt;//id;

	// time to do detection, player is firing!
	if (state == eState::STATE_LETTER_ALIVE) {
		if (Letter::T.y >= 0) {
			float dx = pos.x - Letter::T.x;
			if (fabs(dx) < size.width) {
				//pLabel->setTextColor(Color4B(255, 0, 0, 255));
				// glowing letter
				auto tintTo = TintTo::create(0.3f, 255, 0, 0);
				auto tintTo2 = TintTo::create(0.3f, 0, 0, 0);
				auto seqTint = Sequence::create(tintTo, tintTo2, nullptr);
				pSprite->runAction(RepeatForever::create(seqTint));
				
				state = eState::STATE_LETTER_HIT;
			}
		}

		pos.x -= offset;
		//pos.x -= 3;
		//pos.x--;

		int idx = 2 - (this->belt - 2);
		if (idx >= 0 && idx <= 2) {
			pos.y = startPos.y + wavePos[idx].y;
		}

		// handle wrap-around
		if (pos.x < -width) {
			pos.x += visibleSize.width*1.5f + width * 2; // need to make this wrap around better (for now just *1.5f) TODO
		}
	}
	else if (state == eState::STATE_LETTER_HIT) {
		state = eState::STATE_LETTER_EXPLODE;
		vel.x = (rand() % 101) * 0.1f;
		vel.y = (rand() % 51) * 0.1f;
		oldPos = pos;
		pos += vel;
	}
	else if (state == eState::STATE_LETTER_EXPLODE) {
		if (vel.y > -10.0f) {
			vel.y-= 0.1; // fake gravity
		}
		oldPos = pos;
		pos += vel;
		if (pos.y < 55.0f) {
			pos.y = 55.0f;
			state = eState::STATE_LETTER_GROUND;
		}
	}
	else if (state == eState::STATE_LETTER_GROUND) {
		pos.x -= 3;
		float playerPos = visibleSize.width*0.5f - 300.0f; // hard coded for now (should be able to get the player position instead - TODO)
		float dist = pos.x - playerPos;
		if (fabs(dist) < 16.0f) {
			state = eState::STATE_LETTER_DEAD;
		}
	}
	else if (state == eState::STATE_LETTER_DEAD) {
		status = 0; // for now we will not destroy totally the entity
		pLabel->stopAllActions();
		pSprite->stopAllActions();
		pSprite->setVisible(false);
		pLabel->setVisible(false);
	}

	pSprite->setPosition(pos);

	cyc++;
	seq++;
}

void Letter::draw(void) {
    
}
