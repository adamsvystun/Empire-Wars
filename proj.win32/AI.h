#pragma once
#include "cocos2d.h"
#include "Definitions.h"

class Empire;
class Planet;
class Main;
class Asteroid;

class AI {
public:
	enum state { 
		ATT,//Attack
		RES,//Resourse
		VEN,//Vendeta
		DEF,//Defense
		PAS//Passive
	};
	AI::state state;
	AI(Empire* emp);
	static AI* create(Empire* emp);
	bool init();
	void update();
	void updateAtt(Planet* node);
	void updateAtt(Asteroid* node);
	Planet* vendeta;
	void checkState();
	void doState();
	float prepAtt(Planet* plnt, int n);
	int getAttPotential();
	int countBuyShips(int t);
	bool colPlanet(Planet* plnt);
	Planet* getNearestPlanet(Empire* emp);
	Planet* attPlnt = nullptr;
	Empire* getTopEmp();
	Empire* enemy = nullptr;
	Empire* emp;
private:
};