#ifndef __PLANET_H__
#define __PLANET_H__

#include "ui/CocosGUI.h"
#include "cocos2d.h"
#include "ShipAtt.h"
#include "ShipRes.h"
#include "ShipCol.h"
#include "Definitions.h"

class Empire;
class Main;

class Planet : public cocos2d::Sprite
{
public:
	int type = 4;
	float scale;
	int posx;
	int posy;

	//Label for dragging and selecting ships
	cocos2d::Label* count_lbl;

	//Buy button on the planet 
	cocos2d::ui::Button* btn;
	
	//Constructor and Destructor
	Planet();
	Planet(int t, float scale, int posx, int posy);
	~Planet();

	//Create planet and return pointer
	static Planet* create(int t, float scale, int posx, int posy);
	static Planet* create(int t, int posx, int posy);
	static Planet* create(int posx, int posy);

	//Event for pressing buy button. Shows popup layer.
	void buybtnPressed();

	//Pointer to the empire
	Empire* emp = nullptr;

	//Initialization fuctions
	void initOptions();
	void addEvents();

	//Events for mouse clicks
	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event* event);

	//Retuns length to position x,y
	int get_length(int x, int y);

	//Vectors of all the ships present on the planet
	std::vector<Ship*> ships_att;
	std::vector<ShipCol*> ships_col;
	std::vector<ShipRes*> ships_res;

	//Writing and reading data
	friend std::ostream& operator<<(std::ostream& os, const Planet* plnt);
private:
	//Mouse events helping variables
	bool touched = false;
	bool touchMoved = false;
};

#endif // __PLANET_H__