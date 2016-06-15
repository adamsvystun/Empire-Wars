#include "ShipAtt.h"
#include "Definitions.h"

using namespace cocos2d;

Ship::Ship(float scale, int posx, int posy) {
	this->scale = scale;
	this->posx = posx;
	this->posy = posy;
}

Ship::~Ship() {}
Ship::Ship() {}

Ship* Ship::create(int posx, int posy)
{
	float scale = 0.5f;
	Ship* pSprite = new Ship(scale, posx, posy);
	if (pSprite->initWithFile(pSprite->path))
	{
		pSprite->autorelease();
		pSprite->initOptions();
		pSprite->addEvents();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}

Ship* Ship::create(int posx, int posy, int orbit)
{
	float scale = 0.5f;
	Ship* pSprite = new Ship(scale, posx, posy);
	if (pSprite->initWithFile(pSprite->path))
	{
		pSprite->autorelease();
		pSprite->initOptions(orbit);
		pSprite->addEvents();
		return pSprite;
	}
	CC_SAFE_DELETE(pSprite);
	return NULL;
}

void Ship::initOptions(int orbit)
{	
	if (orbit == 0) {
		this->orbit = SHIP_ORBIT_BASE + rand() % 30;
	}
	else {
		this->orbit = orbit;
	}
	this->setScale(this->scale);
	auto size = this->getContentSize();
	this->setAnchorPoint(Vec2(this->orbit/size.width/this->scale, 0));
	this->setPosition(Vec2(this->posx, this->posy));
	this->setRotation(0.0f+rand()%360);
	auto circulate = RotateBy::create(10.0f + (rand() % 100)/10.f, 360.0f);
	auto repeat = RepeatForever::create(circulate);
	this->runAction(repeat);
}

void Ship::move(int posx, int posy) {
	this->stopAllActions();
	this->setAnchorPoint(Vec2(0.5, 0.5));
	auto pos = this->getPosition();
	double len = sqrt(pow(posx-pos.x, 2)+pow(posy-pos.y,2));
	double koef = this->orbit / len;
	int newx, newy;
	newx = posx-(posx-pos.x)*koef;
	newy = posy - (posy - pos.y)*koef;
	auto move = MoveTo::create(3.0f, Vec2(newx, newy));
	this->runAction(move);
}

void Ship::addEvents()
{
	//auto listener = cocos2d::EventListenerTouchOneByOne::create();
	//listener->setSwallowTouches(true);

	//listener->onTouchBegan = CC_CALLBACK_2(Ship::onTouchBegan, this);
	//listener->onTouchMoved = CC_CALLBACK_2(Ship::onTouchMoved, this);
	//listener->onTouchEnded = CC_CALLBACK_2(Ship::onTouchEnded, this);

	//cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

std::ostream& operator<<(std::ostream& os, const Ship* ship) {
	auto pos = ship->getPosition();
	os << pos.x << " " << pos.y << " " << ship->orbit << std::endl;
	return os;
}

std::istream& operator>>(std::istream  &in, Ship* &ship) {
	return in;
}