//
//  Floor.h
//  game
//
//  Created by Carl Phillips on 21/10/2018.
//
//

#ifndef __game__Floor__
#define __game__Floor__

#include <iostream>
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Floor : public Entity {
public:
	static int total;
	Scene *pParent;
    
	Floor(Scene *parent);
    ~Floor();
    void init(void);
    void spawn(const std::string filename, Vec2 P);
    void deinit(void);
    void process(void);
    void draw(void);
};

#endif /* defined(__game__Floor__) */
