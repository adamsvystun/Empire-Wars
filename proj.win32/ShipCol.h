#pragma once

#include "cocos2d.h"
#include "Definitions.h"
#include "ShipAtt.h"

class Empire;

//Colonization ship
class ShipCol : public Ship
{
public:
	int hp = COL_HP;
	int att = COL_ATT;
	ShipCol(float scale, int posx, int posy);
	~ShipCol();
	static ShipCol* create(int posx, int posy);
	static ShipCol* create(int posx, int posy, int orbit);
	std::string path = "ship2s.png";
};