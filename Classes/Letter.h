//
//  Letter.h
//  game
//
//  Created by Carl Phillips on 21/10/2018.
//
//

#ifndef __game__Letter__
#define __game__Letter__

#include <iostream>
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Letter : public Entity {
public:
	enum eState {
		STATE_LETTER_DEAD = 0,
		STATE_LETTER_ALIVE,
		STATE_LETTER_HIT,
		STATE_LETTER_EXPLODE,
		STATE_LETTER_GROUND
	};
	static Vec2 T; // target position (i.e. we just need the x position of player's laser-beam)
	static Vec2* wavePos;
	Scene *pParent;
	std::string letter;
	Label* pLabel;
	int belt;
	Vec2 startPos;
    
    Letter(Scene *parent);
    ~Letter();
    void init(void);
	void spawn(const std::string letter, int belt, const std::string filename, Vec2 P);
    void deinit(void);
    void process(void);
    void draw(void);
};

#endif /* defined(__game__Letter__) */
