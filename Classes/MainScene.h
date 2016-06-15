#ifndef __MAIN_SCENE_H__
#define __MAIN_SCENE_H__

#include <iostream>
#include <fstream>
#include "cocos2d.h"
#include "Planet.h"
#include "Asteroid.h"
#include "ui/CocosGUI.h"
#include "Definitions.h"
#include "Empire.h"
#include "extensions/GUI/CCControlExtension/CCControlUtils.h"
#include "Exeptions.h"

class Main : public cocos2d::ui::ScrollView
{
public:
	//If this is simulation then true
	bool sim;

	//Creates scene and adds layer(of this class) to it, then returns pointer to it 
    static cocos2d::Scene* createScene(int planetNum, int empireNum, bool sim = false);

	//Loads scene from save file 
	static cocos2d::Scene* loadScene(int n);
	static Main* load(int tag);

	//Creates and initializes instance of this class, then returns pointer to it 
	static Main* create(int planetNum, int empireNum, bool sim = false);

	//Creater menu layer with reuorse counter and ship state slider
	static cocos2d::Layer* createMenu(Main* layer);

	//Menu layer with reuorse counter and ship state slider
	cocos2d::Layer* menu;

	//Ship state slider
	cocos2d::ui::Slider* shipState;

	//Creates popup layer for the ship buying event
	static cocos2d::LayerColor* initPopUp();

	//Shows popup layer for the ship buying event
	void showPopUp(Planet* plnt);

	//Hides popup layer for the ship buying event
	void hidePopUp();

	//Event for buying ships
	void popbtnPressed();

	//Events for sliders in popup layer
	void sliderAttEvent(cocos2d::Ref * sender);
	void sliderColEvent(cocos2d::Ref * sender);
	void sliderResEvent(cocos2d::Ref * sender);

	//Planet which buys ships
	Planet* buy_plnt;

	//How many ships are being bought from popup layer
	int pr_att;
	int pr_res;
	int pr_col;

	//Updates price label in popup layer
	void update_price(cocos2d::Layer* pop);

	//Saves the game
	void saveGame();

	//Keyboard event
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	
	//Initialization functions
	bool init();
	bool initOptions();
	bool initOptionsLoad(int n);
	void loadGame(int n);
	void addEvents();

	//Updates resources and AI's of every empire
	void updateEmpires(float delta);

	//Updates resources labels
	void updateMenu();

	//Pointer to player's empire
	Empire* playerEmpire = nullptr;

	//Pointer to top empire
	Empire* topEmpire;

	//Finds the top empire
	void updateTopEmpire();

	//Deletes empire from the game
	void clearEmp(Empire* emp);

	//Checkes if there is still more than one empire left
	void checkWin();

	//Writing and reading data
	friend std::ostream& operator<<(std::ostream& os, const Main* main);
	
	bool addNode(Empire* emp, int posx, int posy);
	bool addShipToNode(Ship* ship, int posx, int posy);
	bool addResToNode(ShipRes* ship, int posx, int posy);
	bool addColToNode(ShipCol* ship, int posx, int posy);

	//Vectors of all planets, asteroids and empires in the game
	std::vector<Planet*> planets = std::vector<Planet*>();
	std::vector<Asteroid*> asteroids;
	std::vector<Empire*> empires;

	//time of a last battle start
	long long ltime;

	//Number of planets and empires from the PlayMenuScene
	int planetNum = PLANET_N;
	int empireNum;
	
	//Pointes generated for the map
	std::vector<int> xAxis;
	std::vector<int> yAxis;

	//Colors for the empires
	cocos2d::Color3B colorSwatches[8] = { cocos2d::Color3B(255,0,0), cocos2d::Color3B(0,255,0), cocos2d::Color3B(0,0,255), cocos2d::Color3B(255,255,0), cocos2d::Color3B(255,0,255) , cocos2d::Color3B(0,255,255), cocos2d::Color3B(75,75,75), cocos2d::Color3B(150,150,150) };

	//Generates map points
	void genPoints(int minX, int maxX, int minY, int maxY, int points);

	//Generates map points and then puts planets and asteroids on them
	void generateMap();

	//Generates game(map, empires)
	void generateGame();

	//Creates empires for the game
	void generateEmpires();
};

#endif
