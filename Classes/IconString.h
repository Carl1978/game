//
//  IconString.h
//  game
//
//  Created by Carl Phillips on 9/1/2019.
//
//

#ifndef __game__IconString__
#define __game__IconString__

#include <iostream>
#include "cocos2d.h"
#include "Icon.h"

USING_NS_CC;

class IconString {
	float scale;
public:
	Scene* pSceneParent;
	static int idIcon;
	static int idWord;
	std::vector<Icon*> iconArr;
	Size sizeIconDefault;
	float width;
	std::vector<std::string> m_words;

	IconString();
	~IconString();
	void init(void);
	Sprite* spawnSprite(const std::string filename = "B.png", Vec2 P = Vec2::ZERO, Scene* pSceneParent = nullptr, int localZOrder = 0);
	std::vector<Icon*> spawn(const std::string word = "Bonjour", Vec2 P = Vec2::ZERO, Scene* pSceneParent = nullptr, int localZOrder = 0);
	std::vector<Icon*> spawn(std::vector<Icon*> icons, const std::string text = "Bonjour", Vec2 P = Vec2::ZERO, Scene* pSceneParent = nullptr, int localZOrder = 0);
	int getIdxFromIconValue(std::vector<Icon*> iconLetters, const std::string value);
	static std::shared_ptr<IconString> create(const std::string word, Vec2 P = Vec2::ZERO, Scene* pSceneParent = nullptr, int localZOrder = 0);
	static std::shared_ptr<IconString> create(void);
	void setScale(float scale = 1.0f);
	float getScale() {
		return scale;
	}
	float getwidth(void);
	void deinit(void);
	void process(void);
	void draw(void);
	std::vector<std::string> split(const std::string& s, char delimiter);
};

#endif /* defined(__game__IconString__) */
