#include "MainScene.h"
#include "Asteroid.h"

using namespace cocos2d;

Asteroid::Asteroid(float scale, int posx, int posy): scale(scale), posx(posx), posy(posy) {

}

Asteroid::~Asteroid() {}

Asteroid* Asteroid::create(int posx, int posy)
{
	float scale = 0.25f;
	Asteroid* pSprite = new Asteroid(scale, posx, posy);
	std::string path = "planet4.png";

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

void Asteroid::initOptions()
{
	this->setScale(this->scale);
	this->setPosition(Vec2(this->posx, this->posy));

	this->count_lbl = Label::createWithTTF("", "fonts/Marker Felt.ttf", 47);
	this->addChild(this->count_lbl, 0);
}

void Asteroid::addEvents()
{
	auto listener = cocos2d::EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);

	listener->onTouchBegan = CC_CALLBACK_2(Asteroid::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(Asteroid::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(Asteroid::onTouchEnded, this);

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

bool Asteroid::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event* event) {
	cocos2d::Vec2 point = touch->getLocation();
	cocos2d::Rect rect = this->getBoundingBox();

	Main* layer = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	auto pos = layer->getInnerContainerPosition();

	point.x -= pos.x;
	point.y -= pos.y;

	if (rect.containsPoint(point) && layer->playerEmpire != nullptr) {
		this->touched = true;
		return true;
	}

	return false;
}

void Asteroid::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event* event) {
		this->touchMoved = true;
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

		point.x += 37.5f;
		point.y += 37.5f;

		point.x /= this->scale;
		point.y /= this->scale;




		if (this->touched) {
			this->count_lbl->setPosition(point);
			this->count_lbl->setString(len_str);
		}
		
	
}

int Asteroid::get_length(int x, int y) {
	return round(sqrt((this->posx - x)*(this->posx - x) + (this->posy - y)*(this->posy - y)) / 10);
}

void Asteroid::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event* event) {
	Main* layer = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	if (this->touchMoved) {
		if (this->count_lbl->getString() != "") {
			layer->playerEmpire->num_ships_sel = std::stoi(this->count_lbl->getString());
			layer->playerEmpire->plnt_sel = this;
			this->count_lbl->setString("");
		}
	} else {
		layer->playerEmpire->asteroidEvent(this);
	}
	this->touched = false;
	this->touchMoved = false;
}