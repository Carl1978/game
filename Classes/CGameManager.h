//  CGameManager.h

#ifndef __CGameManager__
#define __CGameManager__

#include "cocos2d.h"
#include "Constants.h"
#include "HelloWorldScene.h"

USING_NS_CC;

class CGameManager {
	
public:
	static const int WIDENESS_MORE = 1;
	static const int WIDENESS_SAME = 0;
	static const int WIDENESS_LESS = -1;

	static CGameManager *Instance();
	void deinit();
	~CGameManager();

	void RunScene(int Scene);
	bool bFirstTime;
	void srand(uint32_t _rx, uint32_t _ry, uint32_t _rz, uint32_t _rw);
	uint32_t rand();
	float contentScaleFactor;
	char path[64];
	float aspectRatioDesignSize;
	float aspectRatioScale;
	float designSizeWidth;
	float designSizeHeight;
	cocos2d::Point boundaryOrigin;
	cocos2d::Size boundarySize;
	cocos2d::Size deviceSize; // starting screen size of device / desktop
	int wideness; // if 1 we are wider than the default 320x480 aspect ratio => i.e. 400x480 or 768x1024, etc. 
				  // if 0 we are the same default 320x480
				  // if -1 we are narrower than the default 320x480 aspect ratio => i.e. 300x480 or 1242x2208, etc.
	float fontScale; // 1.0 is default (based on ratio between screen size & design size.
	Scene *m_pScene; // contains layer child (our view)
	std::string m_stringGameType;
	
	void cocos2dLogVec3(char *sTitle, Vec3 *V);
    void update(float dt);

private:
	uint32_t rx, ry, rz, rw;

	CGameManager();
	//CGameManager(){};
	CGameManager(CGameManager const&){};
	CGameManager& operator=(CGameManager const&){};
	static CGameManager* _pInstance;
};

#endif /* defined(__CGameManager__) */
