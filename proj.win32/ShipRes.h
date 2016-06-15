#pragma once

#include "cocos2d.h"
#include "Definitions.h"
#include "ShipAtt.h"

class Empire;

//Resourse ship.
class ShipRes : public Ship
{
public:
	int hp = RES_HP;
	int att = RES_ATT;
	ShipRes(float scale, int posx, int posy);
	~ShipRes();
	static ShipRes* create(int posx, int posy);
	static ShipRes* create(int posx, int posy, int orbit);
	std::string path = "ship3s.png";
};