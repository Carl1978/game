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
						//pSceneParent->removeChild(icon->pSprite); // crashes here, I think we don't need to handle removing
						//delete icon->pSprite;
						//icon->pSprite = nullptr;
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

std::vector<Icon*> IconString::spawn(std::vector<Icon*> icons, const std::string text, Vec2 P, Scene* pSceneParent, int localZOrder) {
	if (icons.size() <= 0) return iconArr;

	std::vector<std::string> sArr;
	size_t size = text.size();
	if (size <= 0) { return iconArr; }

	Icon* pIcon = nullptr;
	Sprite* pSprite = nullptr;
	bool bVar = false;
	std::string sValue = "";

	// special I wrote to check if we have an ASCII char or Unicode char (i.e. 2bytes)
	// NOTE: in some cases this can be 32-bit 4bytes so may have to come back and add platform checking code here later!
	// TODO: refactor into a nice neat helper function
	for (int pos = 0; pos < size;)
	{
		unsigned int c = text.at(pos);
		//cocos2d::log("%c, %d", c, c);
		// single char (in ASCII range)
		if (c < 128) {
			if (bVar == true) { // check for variable (i.e. we are looking for a whole string for a sprite image perhaps)
				if (c == 125) { // }
					bVar = false;
					sArr.push_back(sValue);
					pos++;
					//cocos2d::log("sValue : %s", sValue.c_str());
					sValue = "";
					continue;
				}
				else {
					std::string s = text.substr(pos, 1);
					sValue += s;
				}
			}
			else if (c == 123) { // {
				bVar = true;
			}

			if (!bVar) {
				std::string s = text.substr(pos, 1);
				sArr.push_back(s);
			}
			//cocos2d::log("s: %s", s.c_str());
			pos++;
		}
		// double char needed (unicode range)
		else {
			std::string s = text.substr(pos, 2);
			sArr.push_back(s);
			//cocos2d::log("s: %s", s.c_str());
			pos += 2;
		}
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	for (int i = 0; i < sArr.size(); i++) {
		if (sArr[i].at(0) != 32) { // check we have a valid char (not space " " char)
			// create sprite from an element in icons array
			int idxLetter = getIdxFromIconValue(icons, sArr[i]);

			pSprite = Sprite::createWithTexture(icons.at(idxLetter)->pSprite->getTexture());
			pSprite->setScale(this->scale);
			pSprite->setPosition(P);
			P.x += pSprite->getContentSize().width * pSprite->getScaleX();
			//pSprite->setRotation(45.0f - rand() % 90);

			pIcon = Icon::create(icons.at(idxLetter)->value, pSprite, P);

			iconArr.push_back(pIcon);
		}
		else {
			// for now assume space chars are equal to this width
			P.x += 30.0f * this->scale;
		}
	}

	return iconArr;
}

int IconString::getIdxFromIconValue(std::vector<Icon*> iconLetters, const std::string value) {
	// create an icon "A" from sprite letters vector created above
	Icon* pI = nullptr;
	for (int i = 0; i < iconLetters.size(); i++) {
		pI = iconLetters.at(i);
		if (pI != nullptr) {
			if (pI->value == value) {
				return i;
			}
		}
	}
	return -1;
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