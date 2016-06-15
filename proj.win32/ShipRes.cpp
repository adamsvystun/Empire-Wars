#include "Definitions.h"
#include "ShipRes.h"

using namespace cocos2d;

ShipRes::~ShipRes() {}

ShipRes::ShipRes(float scale, int posx, int posy) {
	this->scale = scale;
	this->posx = posx;
	this->posy = posy;
}

ShipRes* ShipRes::create(int posx, int posy)
{
	float scale = 0.5f;
	ShipRes* pSprite = new ShipRes(scale, posx, posy);
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
ShipRes* ShipRes::create(int posx, int posy, int orbit)
{
	float scale = 0.5f;
	ShipRes* pSprite = new ShipRes(scale, posx, posy);
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
//
//void ShipRes::initOptions()
//{
//	this->orbit = SHIP_ORBIT_BASE + rand() % 30;
//	this->setScale(this->scale);
//	auto size = this->getContentSize();
//	this->setAnchorPoint(Vec2(this->orbit / size.width / this->scale, 0));
//	this->setPosition(Vec2(this->posx, this->posy));
//	this->setRotation(0.0f + rand() % 360);
//	auto circulate = RotateBy::create(10.0f + (rand() % 100) / 10.f, 360.0f);
//	auto repeat = RepeatForever::create(circulate);
//	this->runAction(repeat);
//}
//
//void ShipRes::addEvents() {}