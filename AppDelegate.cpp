#include "AppDelegate.h"
#include "KeyboardScene.h"

USING_NS_CC;

AppDelegate::AppDelegate(){}

AppDelegate::~AppDelegate(){}

bool AppDelegate::applicationDidFinishLaunching() {
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::create("Snake Wars");
		glview->setFrameSize(1200, 600);
		director->setOpenGLView(glview);
	}
	director->setAnimationInterval(0.05f);
	auto scene = KeyboardScene::createScene();
	director->runWithScene(scene);
	return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}
