#include "Planet.h"
#include "Empire.h"
#include "MainScene.h"
#include "Definitions.h"
#include <math.h>

using namespace cocos2d;

Planet::Planet(int t, float scale, int posx, int posy) :type(t), scale(scale), posx(posx), posy(posy) {

}

Planet::~Planet() {}
Planet::Planet() {}

Planet* Planet::create(int posx, int posy) {
	float scale = (float)(PLANET_RADIUS*2.0f/300.0f);
	int t = 1 + (rand() % 3);
	Planet* pSprite = new Planet(t, scale, posx, posy);
	std::string path = "planet" + std::to_string(t) + ".png";

	if (pSprite->initWithFile(path))
	{
		pSprite->autorelease();

		pSprite->initOptions();

		pSprite->addEvents();

		return pSprite;
	}

	CC_SAFE_DELETE(pSprite);
	return NULL;
}

Planet* Planet::create(int t, float scale, int posx, int posy)
{
	Planet* pSprite = new Planet(t, scale, posx, posy);
	std::string path = "planet" + std::to_string(t) + ".png";

	if (pSprite->initWithFile(path))
	{
		pSprite->autorelease();

		pSprite->initOptions();

		pSprite->addEvents();

		return pSprite;
	}

	CC_SAFE_DELETE(pSprite);
	return NULL;
}

Planet* Planet::create(int t, int posx, int posy) {
	float scale = (float)(PLANET_RADIUS*2.0f / 300.0f);
	Planet* pSprite = new Planet(t, scale, posx, posy);
	std::string path = "planet" + std::to_string(t) + ".png";

	if (pSprite->initWithFile(path))
	{
		pSprite->autorelease();

		pSprite->initOptions();

		pSprite->addEvents();

		return pSprite;
	}

	CC_SAFE_DELETE(pSprite);
	return NULL;
}


void Planet::initOptions()
{
	this->setScale(this->scale);
	this->setPosition(Vec2(this->posx,this->posy));

	auto size = this->getContentSize();
	this->btn = ui::Button::create("btn_buy.png", "btn_buy.png", "trans.png");
	this->btn->setPosition(Vec2(size.width/2, size.height / 2));
	this->btn->setScale(1.0f);
	this->btn->setEnabled(false);
	this->btn->addTouchEventListener(CC_CALLBACK_0(Planet::buybtnPressed, this));
	this->addChild(this->btn);

	this->count_lbl = Label::createWithTTF("", "fonts/Marker Felt.ttf", 47);
	this->addChild(this->count_lbl, 0);
}

void Planet::addEvents() {
	auto layer = (cocos2d::ui::ScrollView*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");

	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Planet::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Planet::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Planet::onTouchEnded, this);

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool Planet::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event* event) {
	cocos2d::Vec2 point = touch->getLocation();
	cocos2d::Rect rect = this->getBoundingBox();

	Main* layer = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	auto pos = layer->getInnerContainerPosition();

	point.x -= pos.x;
	point.y -= pos.y;

	if (rect.containsPoint(point) && layer->playerEmpire != nullptr) {
		if (this->emp != nullptr) {
			if (this->emp->player) {
				this->touched = true;
			} else {
				layer->playerEmpire->planetEvent(this);
			}
		} else {
			layer->playerEmpire->planetEvent(this);
		}
		return true;
	}

	return false;
}

void Planet::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event* event) {
	if (this->emp != nullptr) {
		if (this->emp->player) {
			this->touchMoved = true;
			if (this->btn->isEnabled()) {
				this->btn->setEnabled(false);
			}
			cocos2d::Vec2 point = touch->getLocation();
			cocos2d::Rect rect = this->getBoundingBox();

			cocos2d::ui::ScrollView* layer = (cocos2d::ui::ScrollView*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
			auto pos = layer->getInnerContainerPosition();

			point.x -= pos.x;
			point.y -= pos.y;

			int len = this->get_length(point.x, point.y);
			std::string len_str = std::to_string(len);

			point.x -= this->posx;
			point.y -= this->posy;

			point.x += PLANET_RADIUS;
			point.y += PLANET_RADIUS;

			point.x /= this->scale;
			point.y /= this->scale;



			if (this->touched) {
				this->count_lbl->setPosition(point);

				this->count_lbl->setString(len_str);
			}
		}
	}
}

void Planet::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event* event) {
	if (this->emp != nullptr) {
		if (this->emp->player) {
			if (this->touchMoved) {
				if (this->count_lbl->getString()!= "") {
					this->emp->num_ships_sel = std::stoi(this->count_lbl->getString());
					this->emp->plnt_sel = this;
					this->count_lbl->setString("");
				}
			} else {
				Main* layer = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
				layer->playerEmpire->planetEvent(this);
				if (this->btn->isEnabled()) {
					this->btn->setEnabled(false);
				}
				else {
					this->btn->setEnabled(true);
				}
			}
			this->touched = false;
			this->touchMoved = false;
		}
	}
}

void Planet::buybtnPressed() {
	Main* main = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	main->showPopUp(this);
}

int Planet::get_length(int x, int y) {
	return round(sqrt((this->posx-x)*(this->posx - x)+ (this->posy - y)*(this->posy - y))/25);
}

std::ostream& operator<<(std::ostream& os, const Planet* plnt) {
	auto pos = plnt->getPosition();
	os << pos.x << " " << pos.y << " " << plnt->type << std::endl;
	return os;
}