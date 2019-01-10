//
//  GameManager.h
//  game
//
//  Created by Carl Phillips on 18/05/2016.
//
//

#ifndef __game__GameManager__
#define __game__GameManager__

#include <iostream>
#include "cocos2d.h"
#include "EntityManager.h"

USING_NS_CC;

class GameManager {
public:
    Scene *pParent;
    
    char status;
    char state;
    int cyc;
    float seq;
    
    Sprite *pSprite;
    Sprite *pSpriteCurrent;
    
    GameManager(Scene *parent);
    ~GameManager();
    virtual void init(void);
    virtual void spawn(void);
    virtual void deinit(void);
    virtual void process(void);
    virtual void draw(void);
};


#endif /* defined(__game__GameManager__) */
