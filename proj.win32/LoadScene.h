#pragma once
#include "cocos2d.h"
#include <iostream>
#include <fstream>
//Scene for choosing what game to load
class LoadScene : public cocos2d::Layer
{
public:
	//Creates scene and adds layer(of this class) to it, then returns pointer to it 
	static cocos2d::Scene* createScene();

	//Creates and initializes instance of this class, then returns pointer to it 
	static LoadScene* create();

	//Initialization functions
	bool init();
	bool initOptions();
	void addEvents();

private:
	void goToMainScene(cocos2d::Ref * sender);
};