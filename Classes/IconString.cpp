//  IconString.cpp
//  game
//
//  Created by Carl Phillips on 9/1/2019.
//
//

#include "IconString.h"

USING_NS_CC;

int IconString::idIcon;
int IconString::idWord;

IconString::IconString() {
	// TODO:
	cocos2d::log("IconString ctr...");
	sizeIconDefault = Size(32.0f, 32.0f);
	scale = 1.0f;
	width = 0.0f;
	IconString::idIcon = 0;
	IconString::idWord = 0;
}

IconString::~IconString()
{
	cocos2d::log("IconString dtr...");
	if (iconArr.size() > 0) {
		for (auto icon : iconArr) {
			if (icon) {
				if (pSceneParent != nullptr) {
					if (icon->pSprite) {
						cocos2d::log("GOT HERE!");
						//pSceneParent->removeChild(icon->pSprite); // crashes here, I think we don't need to handle removing
						// TODO: something is not getting freed properly!!!!!! Test by restarting each time! May have to turnn off code blocks to locate memory  leak??!!
						//delete icon->pSprite;
						//icon->pSprite = nullptr;
						//int children = pSceneParent->getChildrenCount();
						//cocos2d::log("children : %d", children);
					}
				}
				delete icon;
				icon = nullptr;
			}
		}
		iconArr.clear();
		m_words.clear();
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

	// Create words list (NOTE: idx is the id)
	if (m_words.size() <= 0) {
		m_words = split(text, ' ');
		sArr = split(text, ' ');
		for (auto s : m_words) {
			//	cocos2d::log("s : words : %s", s.c_str());
		}
	}
	else {
		std::vector<std::string> words;
		words = split(text, ' ');
		sArr = split(text, ' ');
		for (auto s : words) {
			//	cocos2d::log("s : words : append : %s", s.c_str());
			m_words.push_back(s);
		}
	}

	Icon* pIcon = nullptr;
	Sprite* pSprite = nullptr;
	bool bVar = false;
	std::string sValue = "";

	// special I wrote to check if we have an ASCII char or Unicode char (i.e. 2bytes)
	// NOTE: in some cases this can be 32-bit 4bytes so may have to come back and add platform checking code here later!
	// TODO: refactor into a nice neat helper function
	//for (int pos = 0; pos < size;)
	//{
	//	unsigned int c = text.at(pos);
	//	//cocos2d::log("%c, %d", c, c);
	//	// single char (in ASCII range)
	//	if (c < 128) {
	//		if (bVar == true) { // check for variable (i.e. we are looking for a whole string for a sprite image perhaps)
	//			if (c == 125) { // }
	//				bVar = false;
	//				sArr.push_back(sValue);
	//				pos++;
	//				//cocos2d::log("sValue : %s", sValue.c_str());
	//				sValue = "";
	//				continue;
	//			}
	//			else {
	//				std::string s = text.substr(pos, 1);
	//				sValue += s;
	//			}
	//		}
	//		else if (c == 123) { // {
	//			bVar = true;
	//		}

	//		if (!bVar) {
	//			std::string s = text.substr(pos, 1);
	//			sArr.push_back(s);
	//		}
	//		//cocos2d::log("s: %s", s.c_str());
	//		pos++;
	//	}
	//	// double char needed (unicode range)
	//	else {
	//		std::string s = text.substr(pos, 2);
	//		sArr.push_back(s);
	//		//cocos2d::log("s: %s", s.c_str());
	//		pos += 2;
	//	}
	//}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	float prevWidth = 0.0f;
	//for (int i = 0; i < sArr.size(); i++) {
		//if (sArr[i].at(0) != 32) { // check we have a valid char (not space " " char)
	for (int i = 0; i < sArr.size(); i++) {
		//if (m_words.at(i) != " ") { // check we have a valid char (not space " " char)
			// create sprite from an element in icons array
			//int idxLetter = getIdxFromIconValue(icons, sArr[i]);
			//pSprite = Sprite::createWithTexture(icons.at(idxLetter)->pSprite->getTexture());
			//m_words
		TTFConfig configTest("fonts/PressStart2P.ttf", 24.0f);
		Label* pLabelTest = Label::createWithTTF(configTest, sArr[i], cocos2d::TextHAlignment::CENTER);
		pLabelTest->setTextColor(Color4B::WHITE);
		pLabelTest->enableOutline(Color4B::BLACK, 6);
		pSprite = createSpriteFromLabel(pLabelTest);

		//pSprite = Sprite::create("boat96x96.png"); // DEBUG
		//pSprite->setVisible(false);//usausa
		pSprite->setScale(this->scale);
		P.x += (prevWidth + pSprite->getContentSize().width) * 0.5f * pSprite->getScaleX();
		pSprite->setPosition(P);
		prevWidth = pSprite->getContentSize().width;
		//pSprite->setRotation(45.0f - rand() % 90);

		//pIcon = Icon::create(icons.at(idxLetter)->value, pSprite, P);
		pIcon = Icon::create(sArr[i], pSprite, P);
		pIcon->idWord = IconString::idWord;
		pIcon->id = IconString::idIcon;
		IconString::idIcon++;

		iconArr.push_back(pIcon);
		//}
		//else {
			// for now assume space chars are equal to this width
		P.x += 60.0f * this->scale;
		IconString::idWord++; // ready for next word
	//}
	}

	// set the current total width of this icon string
	width = getwidth();
	//cocos2d::log("IconString :: width : %f", width);

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

// TODO: not accurate because the first and last positions don't take into consideration the width1*0.5f & width2*0.5f
float IconString::getwidth(void) {
	float smallest = -1.0f, biggest = -1.0f;
	if (iconArr.size() > 0) {
		Icon* pIcon = nullptr;
		float x;
		for (Icon* pIcon : iconArr) {
			if (pIcon) {
				x = pIcon->pSprite->getPosition().x;
				if (smallest < 0 || smallest > x) {
					smallest = x - pIcon->pSprite->getContentSize().width * pIcon->pSprite->getScale() * 0.5f;
				}
				if (biggest < 0 || biggest < x) {
					biggest = x + pIcon->pSprite->getContentSize().width * pIcon->pSprite->getScale() * 0.5f;
				}
			}
		}
	}

	return biggest - smallest;
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

std::vector<std::string> IconString::split(const std::string& s, char delimiter)
{
	std::vector<std::string> tokens;
	std::string token;
	std::istringstream tokenStream(s);
	while (std::getline(tokenStream, token, delimiter))
	{
		tokens.push_back(token);
	}
	return tokens;
}

Sprite* IconString::createSpriteFromLabel(Label* pLabel) {
	if (pLabel == nullptr) { return nullptr; }

	Size pixelSize = Director::getInstance()->getWinSizeInPixels();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Size letterSize = pLabel->getContentSize() + Size(pLabel->getOutlineSize()*2.0f, pLabel->getOutlineSize()*2.0f);
	Vec2 labelPrevPos = pLabel->getPosition();
	float labelPrevScaleY = pLabel->getScaleY();
	RenderTexture* pRenderTexture;
	Sprite* pSprite = nullptr;
	Sprite* pSpriteContainer = Sprite::create();

	pRenderTexture = RenderTexture::create(letterSize.width, letterSize.height, Texture2D::PixelFormat::RGBA8888);
	pRenderTexture->retain();
	pRenderTexture->setKeepMatrix(true);
	pRenderTexture->setVirtualViewport(Vec2(0, 0),
		Rect(0, 0, visibleSize.width, visibleSize.height),
		Rect(0, 0, pixelSize.width, pixelSize.height));

	pSpriteContainer->addChild(pLabel, 0);
	pLabel->setPosition(Vec2(letterSize.width*0.5f, letterSize.height*0.5f));
	pLabel->setScaleY(-1.0f);

	pRenderTexture->beginWithClear(1, 1, 1, 0);
	pSpriteContainer->visit();
	pRenderTexture->end();
	//pRenderTexture->saveToFile("test.png", Image::Format::PNG);

	pLabel->setScaleY(labelPrevScaleY);
	pLabel->setPosition(labelPrevPos);
	pSpriteContainer->removeChild(pLabel, false);

	pSprite = Sprite::createWithTexture(pRenderTexture->getSprite()->getTexture());
	pSprite->retain();
	//pRenderTexture->retain(); // NOTE: the below comment retains made it slow down alot each time I clicked the restart button in app!!!
	//pRenderTexture->getSprite()->retain();
	//pRenderTexture->getSprite()->getTexture()->retain();
	//usa
	//pSprite = Sprite::create();
	//pSprite->setTexture("button-close64x64.png");
	//pSprite = Sprite::create("button-close64x64.png");
	//pSprite->getTexture()->retain();//usa
	//pSprite->initWithTexture(pRenderTexture->getSprite()->getTexture());
	//pSprite->createWithSpriteFrame(pRenderTexture->getSprite()->getSpriteFrame());

	pRenderTexture->release();

	pSprite->setContentSize(letterSize);

	return pSprite;
}

// TODO: words should fade in and fade out at the end of the screen LHS & RHS
//	// glowing lazer beam fx
//	auto fadeTo = FadeTo::create(0.1f, 128.0f);
//	auto fadeTo2 = FadeTo::create(0.1f, 64.0f);
//	auto seqGlow = Sequence::create(fadeTo, fadeTo2, nullptr);
//	m_pSpriteLazer->runAction(RepeatForever::create(seqGlow));