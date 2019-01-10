
//  IconString.cpp
//  game
//
//  Created by Carl Phillips on 9/1/2019.
//
//

#include "IconString.h"

USING_NS_CC;

IconString::IconString() {
	// TODO:
	cocos2d::log("IconString ctr...");
	sizeIconDefault = Size(32.0f, 32.0f);
	scale = 1.0f;
}

IconString::~IconString()
{
	cocos2d::log("IconString dtr...");
	if (iconArr.size() > 0) {
		for (auto icon : iconArr) {
			if (icon) {
				if (pSceneParent != nullptr) {
					if (icon->pSprite) {
						pSceneParent->removeChild(icon->pSprite);
					}
				}
				delete icon;
				icon = nullptr;
			}
		}
		iconArr.clear();
	}
}

Sprite* IconString::spawnSprite(const std::string filename, Vec2 P, Scene* pSceneParent, int localZOrder) {
	Sprite* pSprite = nullptr;
	Icon* pIcon = nullptr;
	this->pSceneParent = pSceneParent;
	pIcon = Icon::create(filename, P);
	if (pIcon != nullptr && pIcon->pSprite != nullptr) {
		pSprite = pIcon->pSprite; // reference to return
		if (pSceneParent != nullptr) {
			pSceneParent->addChild(pIcon->pSprite, localZOrder);
			pIcon->pSprite->setScale(scale);
		}
		iconArr.push_back(pIcon);
	}

	return pSprite;
}

std::vector<Icon*> IconString::spawn(const std::string word, Vec2 P, Scene* pSceneParent, int localZOrder) {
	Icon* pIcon = nullptr;
	char c;
	char buf[256];
	this->pSceneParent = pSceneParent;
	for (int i = 0; i < word.length(); i++) {
		char c = word.c_str()[i];
		// ignore space characters in string
		if (c != ' ') {
			std::sprintf(buf, "%c.png", c);
			pIcon = Icon::create(buf, P + Vec2(sizeIconDefault.width * i * scale, 0.0f));
			if (pIcon != nullptr && pIcon->pSprite != nullptr) {
				if (pSceneParent != nullptr) {
					pSceneParent->addChild(pIcon->pSprite, localZOrder);
					pIcon->pSprite->setScale(scale);
				}
				iconArr.push_back(pIcon);
			}
		}
	}

	return iconArr;
}

std::shared_ptr<IconString> IconString::create(const std::string word, Vec2 P, Scene* pSceneParent, int localZOrder) {
	//IconString* pIconString = new IconString();
	std::shared_ptr<IconString> iconString = std::make_shared<IconString>();
	iconString->spawn(word, P, pSceneParent, localZOrder);
	return iconString;
}

std::shared_ptr<IconString> IconString::create(void) {
	std::shared_ptr<IconString> iconString = std::make_shared<IconString>();
	return iconString;
}

void IconString::setScale(float scale) {
	this->scale = scale;
	if (iconArr.size() > 0) {
		Icon* pIcon = nullptr;
		for (Icon* pIcon : iconArr) {
			pIcon->pSprite->setScale(scale);
		}
	}
}

void IconString::init(void) {
	// TODO:
}

void IconString::deinit(void) {
	// TODO:
}

void IconString::process(void) {
	// TODO:
}

void IconString::draw(void) {
	// TODO:
}
