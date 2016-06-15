#pragma once

#include "cocos2d.h"
//Scene after winning
class WinScene : public cocos2d::Layer
{
public:
	//Create and return scene
	static cocos2d::Scene* createScene(cocos2d::Color3B col);

	//Create layer for the scene
	static WinScene* create(cocos2d::Color3B col);

	//Winning color
	cocos2d::Color3B color;

	//Initialization functions
	bool init();
	bool initOptions();
private:
};