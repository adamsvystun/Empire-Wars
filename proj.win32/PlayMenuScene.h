#pragma once

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class PlayMenuScene : public cocos2d::Layer
{
public:
	//Create and return scene
	static cocos2d::Scene* createScene();

	//Create layer for the scene
	static PlayMenuScene* create();

	//Initialization functions
	bool init();
	bool initOptions();
	void addEvents();
private:
	void goToMainScene(cocos2d::Ref * sender);

	//Go to main scene in simulation mode(No player, only AI)
	void goToMainSceneSimulate(Ref* sender);

	//Numbers of planets and empires choosen
	cocos2d::ui::Slider* planetNum;
	cocos2d::ui::Slider* empireNum;

	//Events for changing labels above sliders
	void planetSliderEvent(cocos2d::Ref * sender);
	void empireSliderEvent(cocos2d::Ref * sender);
};