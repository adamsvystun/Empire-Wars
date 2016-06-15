
#include "LoadScene.h"
#include "MainScene.h"
#include "Definitions.h"

USING_NS_CC;

using namespace cocos2d;

LoadScene* LoadScene::create()
{
	LoadScene *ret = new LoadScene();
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

Scene* LoadScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadScene::create();
	scene->addChild(layer);
	return scene;
}

bool LoadScene::init()
{
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	// creating a Menu from a Vector of items
	Vector<MenuItem*> MenuItems;

	std::fstream f("C:/Users/adams/Desktop/saves/count.txt", std::ios_base::in);
	//if (!f)
	//	throw fileOpenFail();
	int n;
	f >> n;
	f.close();

	log("Number of saves: %d",n);
	for (int i = 0; i < n; ++i) {
		auto menuItemPlay = MenuItemFont::create("Load save "+std::to_string(i), CC_CALLBACK_1(LoadScene::goToMainScene, this));
		menuItemPlay->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height- 50-50*i));
		menuItemPlay->setTag(i);
		MenuItems.pushBack(menuItemPlay);
	}

	auto menu = Menu::createWithArray(MenuItems);
	menu->setPosition(Point::ZERO);
	this->addChild(menu, 1);

	return true;
}

void LoadScene::goToMainScene(Ref * sender) {
	MenuItem* MenuItem = (cocos2d::MenuItem*)(sender);
	int tag = (int)MenuItem->getTag();
	auto scene = Main::loadScene(tag);
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

bool LoadScene::initOptions(){return true;}
void LoadScene::addEvents() {}