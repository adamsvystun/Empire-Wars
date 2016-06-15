#pragma once
#include "cocos2d.h"
#include "Planet.h"
#include "ShipAtt.h"
#include "ShipRes.h"
#include "ShipCol.h"
#include "Definitions.h"
#include "AI.h"

class Asteroid;

class Empire {
public:
	//All ship types - Attaking, Resourse, Colonization
	enum shipType { ATT , RES , COL };

	Empire();

	//Creares empire with color "col" and returns pointer to it
	static Empire* create(cocos2d::ui::ScrollView* layer, cocos2d::Color3B col);

	//Creates AI for the empire
	void createAI();

	//Sets starting planet and puts starting ships on that planet
	void setStartingPlanet(Planet * startPlanet);

	//Creates ship of type "t" and adds it to the planet
	void createShip(Empire::shipType t, int posx, int posy, Planet* Plnt);

	//Creates "n" ships of type "t" and adds them to the planet
	void createShips(Empire::shipType t, int n, int posx, int posy, Planet* Plnt);

	//Add planet to the empire
	bool addNode(int posx, int posy);

	//Planet click event
	void planetEvent(Planet* plnt);

	//Asteroid click event
	void asteroidEvent(Asteroid* ast);

	//Moves ship of type "t" from one planet to another
	template<class F, class T>
	void moveShip(Empire::shipType t, F* fromPlnt, T* toPlnt);

	//Moves "n" ships of type "t" from one planet to another
	template<class F, class T>
	void moveShips(Empire::shipType t, int n, F* fromPlnt, T* toPlnt);

	//Buys ship to the planet
	void buyShips(int att, int res, int col, Planet* plnt);

	//Event for colonizasion ship movement
	void checkCol(Planet* plnt);

	//Deletes "n" ships of type "t" from the planet "plnt"
	template<class F>
	void deleteShips(Empire::shipType t,int n ,F* plnt);

	//Deletes ship of type "t" from the planet "plnt"
	template<class F>
	void deleteShip(Empire::shipType t, F* plnt);

	//Deletes "n" ships which have been destroied after attack
	template<class F>
	void deleteShipsAfterAttack(int n, F* plnt);

	//Event for attack ship movement
	template<class F>
	void checkAtt(F* plnt, int n);

	template<class F>
	void checkAst(F* ast);
	//Event for resourse ship movement
	void checkRes();

	//Returns distance between two planets
	float getDistance(cocos2d::Sprite* plnt1, cocos2d::Sprite* plnt2);

	//Returns distance between two empires(two closest planets of those empires) 
	float getDistance(Empire* emp1, Empire* emp2);

	//Decreses empires resourses
	void descRes(int d, int e, int p);
	
	//Updates empire
	void update(float delta);
	
	//Removes all ships and traces of itself from planet
	void cleanPlanet(Planet* plnt);

	//Removes all ships and traces of itself from asteroid
	void cleanPlanet(Asteroid* plnt);

	//Checks if empire has possibility to continue playing
	void checkLose();

	//Gets defending rating of the planet
	int getPlanetRating(Planet* plnt);

	//Returns nearest planet to empire
	Planet* getNearestPlanet();

	//Retunrs nearest asteroid to empire
	Asteroid* getNearestAsteroid();

	//Returns nearest emoire
	Empire* getNearestEmpire();
	Planet* fromNear;

	//Initialization functions
	bool init();
	void initOptions();

	//Available recourses to the empire
	int diam = START_DIAM;
	int ener = START_ENER;
	int powd = START_POWD;

	//Change in recourses every update
	int diam_delta = PLANET_DIAM;
	int ener_delta = PLANET_ENER;
	int powd_delta = PLANET_POWD;

	//True if this empire is players empire
	bool player = false;

	//Pointer to the AI class
	AI* ai = nullptr;

	//Number of ships selected for movement
	int num_ships_sel = 0;

	//Planet selected from which the movement will begin
	Planet* plnt_sel;

	//Vectors of planets and ships available to the empire
	std::vector<Planet*> planets = std::vector<Planet*>();
	std::vector<Asteroid*> asteroids;
	std::vector<Ship*> ships_att;
	std::vector<ShipCol*> ships_col;
	std::vector<ShipRes*> ships_res;

	//Color of the empire
	cocos2d::Color3B color;

	//Writing and reading data
	friend std::ostream& operator<<(std::ostream& os, const Empire* emp);
private:

	//Pointer to main layer of the game
	cocos2d::ui::ScrollView* layer;
};