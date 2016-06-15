#include "WinScene.h"
#include "Definitions.h"

USING_NS_CC;

using namespace cocos2d;

WinScene* WinScene::create(Color3B col)
{
	WinScene *ret = new WinScene();
	ret->color = col;
	if (ret && ret->init())
	{
		ret->autorelease();
		return ret;
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
}

Scene* WinScene::createScene(Color3B col)
{
	auto scene = Scene::create();
	auto layer = WinScene::create(col);
	scene->addChild(layer);
	return scene;
}

bool WinScene::init()
{
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto winLabel = Label::createWithTTF("This color won! Hurraaayy!", "fonts/Marker Felt.ttf", 56);
	winLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, 500));
	winLabel->setTextColor(Color4B(this->color));
	winLabel->enableGlow(Color4B(this->color));
	this->addChild(winLabel);

	//auto menuItemPlay = MenuItemFont::create("Start", CC_CALLBACK_1(PlayMenuScene::goToMainScene, this));
	//menuItemPlay->setPosition(Vec2(visibleSize.width / 2 + origin.x, 150/*visibleSize.height / 2 + origin.y*/));

	//auto menuItemSimulate = MenuItemFont::create("Simulate", CC_CALLBACK_1(PlayMenuScene::goToMainSceneSimulate, this));
	//menuItemSimulate->setPosition(Vec2(visibleSize.width / 2 + origin.x, 50/*visibleSize.height / 2 + origin.y*/));

	//auto menu = Menu::create(menuItemPlay, menuItemSimulate, NULL);
	//menu->setPosition(Point::ZERO);

	//this->addChild(menu);

	return true;
}

bool WinScene::initOptions(){return true;}