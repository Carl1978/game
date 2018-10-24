//  CGameManager.cpp

#include "CGameManager.h"
#include "Constants.h"
#include "HelloWorldScene.h"

USING_NS_CC;

CGameManager *CGameManager::_pInstance = NULL;

CGameManager *CGameManager::Instance() {
	if(!_pInstance) {
		_pInstance = new CGameManager;
		_pInstance->bFirstTime = true; //  for doing first time stuff
		_pInstance->contentScaleFactor = 1.0; // default (assuming 1024x768)
		_pInstance->designSizeWidth = 1024.0f;
		_pInstance->designSizeHeight = 768.0f;
		_pInstance->aspectRatioDesignSize = _pInstance->designSizeWidth / _pInstance->designSizeHeight; // default (assuming 1024x768)
		_pInstance->aspectRatioScale = 1.0; // default (assuming 1024x768)
		_pInstance->wideness = WIDENESS_SAME;
		_pInstance->boundaryOrigin = Point(0, 0);
		_pInstance->boundarySize = Size(1024, 768);
		_pInstance->deviceSize = Size(1024, 768);
		_pInstance->fontScale = 1.0; // default (assuming 1024x768)
		_pInstance->m_stringGameType = "Game"; // default
    }
	return _pInstance;
}

void CGameManager::deinit() {
	if (_pInstance) {
		delete _pInstance;
		_pInstance = NULL;
	}
}

CGameManager::CGameManager() {	
	cocos2d::log("CGameManager::CGameManager()...");
}

CGameManager::~CGameManager() {
	cocos2d::log("CGameManager::~CGameManager()...");
}

void CGameManager::RunScene(int scene) {
	float fadeSec = 0.3;

	// choose scene (creating MVC)
	switch (scene) {
		/////////////////////////////////////////////////////////////////////////////////////////////////////
		case kViewGame:
			// create view
			m_pScene = HelloWorld::createScene();
			Director::getInstance()->setDisplayStats(false);
			break;
		/////////////////////////////////////////////////////////////////////////////////////////////////////
	}

	if(CCDirector::sharedDirector()->getRunningScene() == NULL) {
		//CCDirector::sharedDirector()->runWithScene(m_pScene);
		CCDirector::sharedDirector()->runWithScene(TransitionFade::create(fadeSec, m_pScene, Color3B(0, 0, 0)));
	} else {
		//CCDirector::sharedDirector()->replaceScene(m_pScene);
		CCDirector::sharedDirector()->replaceScene(TransitionFade::create(fadeSec, m_pScene, Color3B(0, 0, 0)));
	}
}

void CGameManager::update(float) {}

void CGameManager::srand(uint32_t _rx, uint32_t _ry, uint32_t _rz, uint32_t _rw) {
	this->rx = _rx;
	this->ry = _ry;
	this->rz = _rz;
	this->rw = _rw;
}

uint32_t rx, ry, rz, rw;
uint32_t CGameManager::rand() {
	uint32_t t = rx ^ (rx << 11);
	rx = ry; ry = rz; rz = rw;
	return rw = rw ^ (rw >> 19) ^ t ^ (t >> 8);
}

void CGameManager::cocos2dLogVec3(char *sTitle, Vec3 *V) {
	cocos2d::log("%s : x: %f, y: %f, z: %f", sTitle, V->x, V->y, V->z);
}
