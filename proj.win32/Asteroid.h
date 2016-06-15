#ifndef __ASTEROID_H__
#define __ASTEROID_H__

#include "cocos2d.h"
#include "Planet.h"

class Main;

//Asteroid class
class Asteroid : public Planet
{
public:
	float scale;
	int posx;
	int posy;
	int type = 4;
	bool touched = false;
	bool touchMoved = false;

	cocos2d::Label* count_lbl;

	Asteroid(float scale, int posx, int posy);
	~Asteroid();
	static Asteroid* create(int posx, int posy);

	void initOptions();

	void addEvents();

	int get_length(int x, int y);

	bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event* event);

private:
};

#endif // __ASTEROID_H__