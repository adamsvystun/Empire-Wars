#ifndef __SHIP_H__
#define __SHIP_H__


#include "cocos2d.h"
#include "Definitions.h"

class Empire;


//Attack ship
class Ship : public cocos2d::Sprite
{
public:
	float scale;

	//Position of the ship on the map
	int posx;
	int posy;

	//Length form center of the planet to the ship
	int orbit;

	//Path to the ship in=mage
	std::string path = "ship1s.png";

	//Pointer to the empire, whose this ship instance is.
	Empire* emp;

	//Health and attack points
	int hp = ATT_HP;
	int att = ATT_ATT;

	//Constuctor and Destructor
	Ship();
	Ship(float scale, int posx, int posy);
	~Ship();

	//Create ship and return pointer to it.
	static Ship* create(int posx, int posy);
	static Ship* create(int posx, int posy, int orbit);

	//Move ship
	void move(int posx, int posy);

	//Initialization functions
	void initOptions(int orbit = 0);
	void addEvents();

	//bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event* event);
	//void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event* event);
	//void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event* event);

	//Writing and reading data
	friend std::ostream& operator<<(std::ostream& os, const Ship* ship);
	friend std::istream& operator>>(std::istream& in, Ship* &ship);
private:
};
#endif