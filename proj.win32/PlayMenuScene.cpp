#include "PlayMenuScene.h"
#include "MainScene.h"
#include "Definitions.h"

USING_NS_CC;

using namespace cocos2d;

PlayMenuScene* PlayMenuScene::create()
{
	PlayMenuScene *ret = new PlayMenuScene();
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

Scene* PlayMenuScene::createScene()
{
	auto scene = Scene::create();
	auto layer = PlayMenuScene::create();
	scene->addChild(layer);
	return scene;
}

bool PlayMenuScene::init()
{
	if (!Layer::init()) {
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto planetLabel = Label::createWithTTF("Number of planets: 8", "fonts/Marker Felt.ttf", 32);
	planetLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, 350));
	planetLabel->setName("plab");
	this->addChild(planetLabel);

	auto empireLabel = Label::createWithTTF("Number of empires: 2", "fonts/Marker Felt.ttf", 32);
	empireLabel->setPosition(Vec2(visibleSize.width / 2 + origin.x, 450));
	empireLabel->setName("elab");
	this->addChild(empireLabel);

	this->planetNum = ui::Slider::create();
	this->planetNum->loadBarTexture("Default/Slider_Back.png");
	this->planetNum->loadSlidBallTextures("Default/SliderNode_Normal.png", "Default/SliderNode_Press.png", "");
	this->planetNum->loadProgressBarTexture("Default/Slider_PressBar.png");
	this->planetNum->setScale9Enabled(true);
	this->planetNum->setMaxPercent(40);
	this->planetNum->setCapInsets(Rect(0, 0, 0, 0));
	this->planetNum->setContentSize(Size(250.0f, 19));
	this->planetNum->setPosition(Vec2(visibleSize.width / 2 + origin.x, 300));
	this->planetNum->addEventListener(CC_CALLBACK_1(PlayMenuScene::planetSliderEvent, this));

	this->addChild(this->planetNum);

	this->empireNum = ui::Slider::create();
	this->empireNum->loadBarTexture("Default/Slider_Back.png");
	this->empireNum->loadSlidBallTextures("Default/SliderNode_Normal.png", "Default/SliderNode_Press.png", "");
	this->empireNum->loadProgressBarTexture("Default/Slider_PressBar.png");
	this->empireNum->setScale9Enabled(true);
	this->empireNum->setMaxPercent(10);
	this->empireNum->setCapInsets(Rect(0, 0, 0, 0));
	this->empireNum->setContentSize(Size(250.0f, 19));
	this->empireNum->setPosition(Vec2(visibleSize.width / 2 + origin.x, 400));
	this->empireNum->addEventListener(CC_CALLBACK_1(PlayMenuScene::empireSliderEvent, this));

	this->addChild(this->empireNum);

	auto menuItemPlay = MenuItemFont::create("Start", CC_CALLBACK_1(PlayMenuScene::goToMainScene, this));
	menuItemPlay->setPosition(Vec2(visibleSize.width / 2 + origin.x, 150/*visibleSize.height / 2 + origin.y*/));

	auto menuItemSimulate = MenuItemFont::create("Simulate", CC_CALLBACK_1(PlayMenuScene::goToMainSceneSimulate, this));
	menuItemSimulate->setPosition(Vec2(visibleSize.width / 2 + origin.x, 50/*visibleSize.height / 2 + origin.y*/));

	auto menu = Menu::create(menuItemPlay, menuItemSimulate,  NULL);
	menu->setPosition(Point::ZERO);

	this->addChild(menu);

	return true;
}
void PlayMenuScene::planetSliderEvent(cocos2d::Ref * sender) {
	Label* planetLabel = (Label*)this->getChildByName("plab");
	int planetN = this->planetNum->getPercent()+ MIN_PLANET;
	std::string label = "Number of planets: " + std::to_string(planetN);
	planetLabel->setString(label);
}
void PlayMenuScene::empireSliderEvent(cocos2d::Ref * sender) {
	Label* empireLabel = (Label*)this->getChildByName("elab");
	int empireN = this->empireNum->getPercent()+ MIN_EMPIRE;
	std::string label = "Number of empires: " + std::to_string(empireN);
	empireLabel->setString(label);
}

void PlayMenuScene::goToMainScene(Ref * sender) {
	int planetN = this->planetNum->getPercent() + MIN_PLANET;
	int empireN = this->empireNum->getPercent() + MIN_EMPIRE;
	auto scene = Main::createScene(planetN, empireN);
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

void PlayMenuScene::goToMainSceneSimulate(Ref* sender) {
	int planetN = this->planetNum->getPercent() + MIN_PLANET;
	int empireN = this->empireNum->getPercent() + MIN_EMPIRE;
	auto scene = Main::createScene(planetN, empireN, true);
	Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
}

bool PlayMenuScene::initOptions()
{

	return true;
}

void PlayMenuScene::addEvents() {}
