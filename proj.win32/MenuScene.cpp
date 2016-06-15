#include "MenuScene.h"
#include "PlayMenuScene.h"
#include "LoadScene.h"
#include "Definitions.h"

USING_NS_CC;

using namespace cocos2d;

MenuScene* MenuScene::create()
{
	MenuScene *ret = new MenuScene();
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

Scene* MenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = MenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool MenuScene::init()
{
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto game_logo = Label::createWithTTF("Empire WARS", "fonts/Marker Felt.ttf", 62);
	game_logo->setTextColor(Color4B(0, 191, 255, 255));
	game_logo->enableGlow(Color4B(0, 191, 255, 255));
	game_logo->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y+100));
	this->addChild(game_logo, 0);

	auto menuItemPlay = MenuItemFont::create("Play", CC_CALLBACK_1(MenuScene::goToPlayMenuScene, this));
	menuItemPlay->setPosition(Vec2(visibleSize.width/2+origin.x, visibleSize.height/2+origin.y));

	auto menuItemLoad = MenuItemFont::create("Load", CC_CALLBACK_1(MenuScene::goToLoadScene, this));
	menuItemLoad->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y - 100));

	auto menu = Menu::create(menuItemPlay, menuItemLoad, NULL);
	menu->setPosition(Point::ZERO);

	this->addChild(menu);

	return true;
}

void MenuScene::goToPlayMenuScene(Ref * sender) {
	auto scene = PlayMenuScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void MenuScene::goToLoadScene(Ref * sender) {
	auto scene = LoadScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

bool MenuScene::initOptions()
{

	return true;
}

void MenuScene::addEvents() {}
