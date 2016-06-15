#include "AppDelegate.h"
#include "MainScene.h"
#include "MenuScene.h"
#include "Definitions.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() {

}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("net.svit.planets");
		glview->setFrameSize(WINDOW_WIDTH, WINDOW_HEIGHT);
        director->setOpenGLView(glview);
    }

    auto scene = MenuScene::createScene();
    director->runWithScene(scene);
    return true;
}


void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {

}