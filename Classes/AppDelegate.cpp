/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "CGameManager.h"

 // #define USE_AUDIO_ENGINE 1
 // #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
	CGameManager::Instance()->deinit();
#if USE_AUDIO_ENGINE
	AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

	GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		//        glview = GLViewImpl::createWithRect("game", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));

				//glview = GLViewImpl::create("game");
				//glview = GLViewImpl::createWithFullScreen("game");
				//glview = GLViewImpl::create("game", true);
				//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 1024, 640), 1.0f); // Windows 10 OS testing
				//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 1024, 640), 1.0f, true); // Windows 10 OS testing
		glview = GLViewImpl::createWithRect("game", Rect(0, 0, 853, 480), 1.0f, false); // Android iLike x3 pro slim
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 480, 320), 1.0f, true); // Android Samsung Galaxy Mini 2 S6500
		// IMPORTANT: If you use physics with debug draw enabled. Don't scale the view here !!! Causes graphical offsets between Sprite3D and physics objects!
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 1024, 768), 0.8f); // iPad, iPad 2, iPad Mini
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 1024, 768), 1.0f, true); // iPad, iPad 2, iPad Mini
		//++glview = GLViewImpl::createWithRect("game", Rect(0, 0, 1280, 800), 0.5f); // Android Tablet Jalcor Access Q88IM
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 960, 640), 1.0f); // iPhone 4 (Retina mode)
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 1024, 768), 1.0f); // iPad, iPad 2, iPad Mini
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 2048, 1536), 0.25f); // iPad Air, iPad Mini Retina
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 2048, 1536), 1.0f); // iPad Air, iPad Mini Retina
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 480, 320)); // iPhone 3GS (3G, 3, iPod) set dimension here

		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 920, 690), 1.0f); // iSpecial (landscape view actually)
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 690, 920), 1.0f); // iSpecial (portrait view actually)
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 640, 640), 1.0f); // iSquare
		//glview = GLViewImpl::createWithRect("game", Rect(0, 0, 512, 400), 1.0f); // half size iPad, iPad 2, iPad Mini
#else
		glview = GLViewImpl::create("game");
#endif
		director->setOpenGLView(glview);
	}

	// calculate screen useful data
	Size screenSize = glview->getFrameSize();
	CGameManager::Instance()->deviceSize = screenSize;
	CGameManager::Instance()->designSizeWidth = 1024;
	CGameManager::Instance()->designSizeHeight = 768;
	Size designSize = Size(CGameManager::Instance()->designSizeWidth, CGameManager::Instance()->designSizeHeight);
	Size resourceSize;

	CGameManager::Instance()->aspectRatioDesignSize = CGameManager::Instance()->designSizeWidth / CGameManager::Instance()->designSizeHeight;
	CGameManager::Instance()->aspectRatioScale = 1.0;
	resourceSize = Size(1024, 768);
	cocos2d::log("ipad / iphone5 / iphone 6  / iphone 6+, etc...");

	// add font scale
	//float fontScale = resourceSize.width / designSize.width;//screenSize.width / designSize.width;
	float fontScale = 1.0f;// screenSize.width / designSize.width; // same as frameSize.width / designSize.width
	cocos2d::log("fontScale : %f", fontScale);
	CGameManager::Instance()->fontScale = fontScale;

	// add search paths (folder locations of various resources / assets)
	CCFileUtils *pFileUtils = CCFileUtils::sharedFileUtils();
	std::vector<std::string> searchPaths;
	// Mac OS desktop
#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	searchPaths.push_back("res");
	searchPaths.push_back("res/biomessres");
	searchPaths.push_back("res/pngcards");
	searchPaths.push_back("res/panel");
	searchPaths.push_back("res/sounds");
	searchPaths.push_back("res/letters");
	searchPaths.push_back("res/freeassets");
#else
	searchPaths.push_back("biomessres");
	searchPaths.push_back("sounds");
	searchPaths.push_back("letters");
	searchPaths.push_back("freeassets");
#endif
	pFileUtils->setSearchPaths(searchPaths);

	//director->setContentScaleFactor(resourceSize.height / designSize.height);
	director->setContentScaleFactor(resourceSize.width / designSize.width);

	//glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::FIXED_HEIGHT);
	//glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::NO_BORDER);
	//glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);
	//glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::SHOW_ALL);

	CGameManager::Instance()->contentScaleFactor = director->getContentScaleFactor();

	// debug stats
	cocos2d::log("screenSize.width : %f", screenSize.width);
	cocos2d::log("screenSize.height : %f", screenSize.height);
	cocos2d::log("resourceSize.width : %f", resourceSize.width);
	cocos2d::log("resourceSize.height : %f", resourceSize.height);
	cocos2d::log("designSize.width : %f", designSize.width);
	cocos2d::log("designSize.height : %f", designSize.height);
	cocos2d::log("CGameManager::Instance()->aspectRatioDesignSize : %f", CGameManager::Instance()->designSizeWidth);
	cocos2d::log("CGameManager::Instance()->aspectRatioDesignSize : %f", CGameManager::Instance()->designSizeHeight);
	cocos2d::log("CGameManager::Instance()->aspectRatioDesignSize : %f", CGameManager::Instance()->aspectRatioDesignSize);
	cocos2d::log("CGameManager::Instance()->wideness : %d", CGameManager::Instance()->wideness);

	Size visibleSize = director->getVisibleSize();
	Point origin = director->getVisibleOrigin();
	Size winSizeInPixels = director->getWinSizeInPixels();
	cocos2d::log("origin.width : %f, origin.height : %f", origin.x, origin.y);
	cocos2d::log("visibleSize.width : %f, visibleSize.height : %f", visibleSize.width, visibleSize.height);
	cocos2d::log("winSizeInPixels.width : %f, winSizeInPixels.height : %f", winSizeInPixels.width, winSizeInPixels.height);
	//boundarySize = Size(visibleSize.height*CGameManager::Instance()->aspectRatioDesignSize, visibleSize.height);

	CGameManager::Instance()->boundaryOrigin = Point((visibleSize.width - CGameManager::Instance()->designSizeWidth)*0.5,
		(visibleSize.height - CGameManager::Instance()->designSizeHeight));
	CGameManager::Instance()->boundarySize = Size(CGameManager::Instance()->designSizeWidth, CGameManager::Instance()->designSizeHeight);
	cocos2d::log("boundaryOrigin.x : %f, boundaryOrigin.y : %f", CGameManager::Instance()->boundaryOrigin.x,
		CGameManager::Instance()->boundaryOrigin.y);
	cocos2d::log("boundarySize.width : %f, boundarySize.height : %f", CGameManager::Instance()->boundarySize.width,
		CGameManager::Instance()->boundarySize.height);

	// turn on display FPS
	director->setDisplayStats(true);

	// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	/*
	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
	auto frameSize = glview->getFrameSize();
	// if the frame's height is larger than the height of medium size.
	if (frameSize.height > mediumResolutionSize.height)
	{
		director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
	}
	// if the frame's height is larger than the height of small size.
	else if (frameSize.height > smallResolutionSize.height)
	{
		director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
	}
	// if the frame's height is smaller than the height of medium size.
	else
	{
		director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
	}
	*/

	register_all_packages();

	// create a scene. it's an autorelease object
	//auto scene = HelloWorld::createScene();

	// run
	//director->runWithScene(scene);
	//CGameManager::Instance()->RunScene(kViewHello);
	//CGameManager::Instance()->RunScene(kViewGame);
	//CGameManager::Instance()->RunScene(kViewSnake);
	CGameManager::Instance()->RunScene(kViewMapTap);

	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}