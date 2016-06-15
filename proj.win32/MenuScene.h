#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

//Start menu scene
class MenuScene : public cocos2d::Layer
{
public:
	//Creates scene and adds layer(of this class) to it, then returns pointer to it 
	static cocos2d::Scene* createScene();

	//Creates and initializes instance of this class, then returns pointer to it 
	static MenuScene* create();	

	//Initialization functions
	bool init();
	bool initOptions();
	void addEvents();

private:
	void goToPlayMenuScene(cocos2d::Ref * sender);
	void goToLoadScene(cocos2d::Ref * sender);
};

#endif