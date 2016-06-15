#include "Empire.h"
#include "MainScene.h"
#include "Definitions.h"
#include <algorithm>

using namespace cocos2d;

Empire::Empire() {

}

bool Empire::init() {
	return true;
}

Empire* Empire::create(cocos2d::ui::ScrollView* layer, Color3B col) {
	try {
		Empire* emp = new Empire();
		emp->layer = layer;
		emp->color = col;
		if (emp && emp->init())
		{
			emp->initOptions();
			return emp;
		}
	} catch (const std::bad_alloc& e) {
		return nullptr;
	}
}

void Empire::initOptions() {
	
}

void Empire::createAI() {
	this->ai = AI::create(this);
}

void Empire::update(float delta) {
	this->ai->update();
}

void Empire::setStartingPlanet(Planet * startPlanet) {
	startPlanet->setColor(this->color);

	auto pos = startPlanet->getPosition();
	this->planets.push_back(startPlanet);
	
	this->createShips(Empire::shipType::ATT, START_SHIP_N, pos.x, pos.y, startPlanet);
	this->createShip(Empire::shipType::RES, pos.x, pos.y, startPlanet);
	this->checkRes();
}

void Empire::createShips(Empire::shipType t, int n, int posx, int posy, Planet* Plnt) {
	for (int i = 0; i < n; i++) {
		this->createShip(t, posx, posy, Plnt);
	}
}

void Empire::createShip(Empire::shipType t, int posx, int posy, Planet* Plnt) {
	if (t == Empire::shipType::ATT) {
		Ship* ship = Ship::create(posx, posy);
		ship->emp = this;
		this->ships_att.push_back(ship);
		Plnt->ships_att.push_back(ship);
		this->layer->addChild(ship, 0);
	}
	else if (t == Empire::shipType::COL) {
		ShipCol* ship = ShipCol::create(posx, posy);
		ship->emp = this;
		this->ships_col.push_back(ship);
		Plnt->ships_col.push_back(ship);
		this->layer->addChild(ship, 0);
	}
	else {
		ShipRes* ship = ShipRes::create(posx, posy);
		ship->emp = this;
		this->ships_res.push_back(ship);
		Plnt->ships_res.push_back(ship);
		this->layer->addChild(ship, 0);
	}
}

std::ostream& operator<<(std::ostream& os, const Empire* emp) {
	auto col = emp->color;
	os << int(col.r) << " " << int(col.g) << " " << int(col.b);
	os << " " << emp->diam << " " << emp->ener << " " << emp->powd << std::endl;
	os << emp->planets.size() << std::endl;
	for (int i = 0; i != emp->planets.size(); ++i) {
		os << emp->planets[i];
	}
	os << emp->asteroids.size() << std::endl;
	for (int i = 0; i != emp->asteroids.size(); ++i) {
		os << emp->asteroids[i];
	}
	os << emp->ships_att.size() << std::endl;
	for (int i = 0; i != emp->ships_att.size(); ++i) {
		os << emp->ships_att[i];
		if (emp->ships_att[i]->getNumberOfRunningActions() > 1) {
			throw staticShip();
		}
	}
	os << emp->ships_res.size() << std::endl;
	for (int i = 0; i != emp->ships_res.size(); ++i) {
		os << emp->ships_res[i];
		if (emp->ships_res[i]->getNumberOfRunningActions() > 1) {
			throw staticShip();
		}
	}
	os << emp->ships_col.size() << std::endl;
	for (int i = 0; i != emp->ships_col.size(); ++i) {
		os << emp->ships_col[i];
		if (emp->ships_col[i]->getNumberOfRunningActions() > 1) {
			throw staticShip();
		}
	}
	return os;
}

bool Empire::addNode(int posx, int posy) {
	Main* main = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	for (std::vector<Planet*>::iterator it = main->planets.begin(); it != main->planets.end(); ++it) {
		auto pos = (*it)->getPosition();
		if (pos.x == posx && pos.y == posy) {
			(*it)->emp = this;
			(*it)->setColor(this->color);
			this->planets.push_back((*it));
			return true;
		}
	}
	/*for (int i = 0; i != main->planets.size(); ++i) {
		auto pos = main->planets[i]->getPosition();
		if (pos.x == posx && pos.y == posy) {
			main->planets[i]->emp = this;
			main->planets[i]->setColor(this->color);
			this->planets.push_back(main->planets[i]);
			return true;
		}
	}
	for (int i = 0; i != main->asteroids.size(); ++i) {
		auto pos = main->asteroids[i]->getPosition();
		if (pos.x == posx && pos.y == posy) {
			main->asteroids[i]->emp = this;
			this->asteroids.push_back(main->asteroids[i]);
			return true;
		}
	}*/
	return false;
}

void Empire::planetEvent(Planet* plnt) {
	if (this->num_ships_sel != 0) {
		Main* layer = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
		switch (layer->shipState->getPercent()){
		case 0:
			if (this->plnt_sel->ships_att.size() >= this->num_ships_sel) {
				this->moveShips(Empire::shipType::ATT, this->num_ships_sel, this->plnt_sel, plnt);
			} else if (this->plnt_sel->ships_att.size() != 0) {
				this->moveShips(Empire::shipType::ATT, this->plnt_sel->ships_att.size(), this->plnt_sel, plnt);
			}
			break;
		case 1:
			if (this->plnt_sel->ships_res.size() >= this->num_ships_sel) {
				this->moveShips(Empire::shipType::RES, this->num_ships_sel, this->plnt_sel, plnt);
			} else if (this->plnt_sel->ships_res.size() != 0) {
				this->moveShips(Empire::shipType::RES, this->plnt_sel->ships_res.size(), this->plnt_sel, plnt);
			}
			break;
		default:
			if (this->plnt_sel->ships_col.size() >= this->num_ships_sel) {
				this->moveShips(Empire::shipType::COL, this->num_ships_sel, this->plnt_sel, plnt);
			} else if (this->plnt_sel->ships_col.size() != 0) {
				this->moveShips(Empire::shipType::COL, this->plnt_sel->ships_col.size(), this->plnt_sel, plnt);
			}
			break;
		}
	}
}

void Empire::asteroidEvent(Asteroid* ast) {
	if (this->num_ships_sel != 0) {
		Main* layer = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
		switch (layer->shipState->getPercent()) {
		case 0:
			if (this->plnt_sel->ships_att.size() >= this->num_ships_sel) {
				this->moveShips(Empire::shipType::ATT, this->num_ships_sel, this->plnt_sel, ast);
			}
			else if (this->plnt_sel->ships_att.size() != 0) {
				this->moveShips(Empire::shipType::ATT, this->plnt_sel->ships_att.size(), this->plnt_sel, ast);
			}
			break;
		case 1:
			if (this->plnt_sel->ships_res.size() >= this->num_ships_sel) {
				this->moveShips(Empire::shipType::RES, this->num_ships_sel, this->plnt_sel, ast);
			}
			else if (this->plnt_sel->ships_res.size() != 0) {
				this->moveShips(Empire::shipType::RES, this->plnt_sel->ships_res.size(), this->plnt_sel, ast);
			}
			break;
		default:
			if (this->plnt_sel->ships_col.size() >= this->num_ships_sel) {
				this->moveShips(Empire::shipType::COL, this->num_ships_sel, this->plnt_sel, ast);
			}
			else if (this->plnt_sel->ships_col.size() != 0) {
				this->moveShips(Empire::shipType::COL, this->plnt_sel->ships_col.size(), this->plnt_sel, ast);
			}
			break;
		}
	}
}

template<class F, class T>
void Empire::moveShips(Empire::shipType t,int n, F* fromPlnt, T* toPlnt) {
	float dist = this->getDistance(fromPlnt, toPlnt);
	float time = dist / SHIP_SPEED;
	for (int i = 0; i < n; i++){
		this->moveShip(t,fromPlnt,toPlnt);
	}
	this->num_ships_sel = 0;
	CallFunc* callback;
	if (t == Empire::shipType::ATT) {
		callback = CallFunc::create([this, toPlnt, n]() {
			this->checkAtt(toPlnt, n);
		});
	} else if (t == Empire::shipType::RES) {
		if (toPlnt->type == 4) {
			callback = CallFunc::create([this, toPlnt]() {
				this->checkAst(toPlnt);
				this->checkRes();
			});
		}
		else {
			callback = CallFunc::create([this, toPlnt]() {
				this->checkRes();
			});
		}
	} else {
		if (toPlnt->type == 4) {
			callback = CallFunc::create([this, toPlnt]() {
				this->checkAst(toPlnt);
			});
		} else {
			callback = CallFunc::create([this, toPlnt]() {
				this->checkCol(toPlnt);
			});
		}
	}
	auto seq = Sequence::create(DelayTime::create(time), callback, nullptr);
	Director::getInstance()->getRunningScene()->runAction(seq);
}

template<class F, class T>
void Empire::moveShip(Empire::shipType t, F* fromPlnt, T* toPlnt) {
	float dist = this->getDistance(fromPlnt, toPlnt);
	float time = dist/SHIP_SPEED;
	if (t == Empire::shipType::ATT) {
		Ship* ship = fromPlnt->ships_att.back();
		fromPlnt->ships_att.pop_back();

		auto callback = CallFunc::create([toPlnt, ship]() {
			toPlnt->ships_att.push_back(ship);
		});
		auto moveTo = MoveTo::create(time, Vec2(toPlnt->posx, toPlnt->posy));
		
		auto seq = Sequence::create(moveTo, callback, nullptr);
		ship->runAction(seq);
	}
	else if (t == Empire::shipType::COL) {
		ShipCol* ship = fromPlnt->ships_col.back();
		fromPlnt->ships_col.pop_back();

		auto callback = CallFunc::create([toPlnt, ship]() {
			toPlnt->ships_col.push_back(ship);
		});
		auto moveTo = MoveTo::create(time, Vec2(toPlnt->posx, toPlnt->posy));

		auto seq = Sequence::create(moveTo, callback, nullptr);
		ship->runAction(seq);

		/*auto moveTo = MoveTo::create(time, Vec2(toPlnt->posx, toPlnt->posy));

		cocos2d::CallFunc* callback;

		if (toPlnt->type == 4) {
			auto callback = CallFunc::create([this, toPlnt]() {
				this->checkAst(toPlnt);
			});
		} else {
			auto callback = CallFunc::create([this, toPlnt]() {
				this->checkCol(toPlnt);
			});
		}
		auto seq = Sequence::create(moveTo, callback, nullptr);

		ship->runAction(seq);*/
	}
	else {
		ShipRes* ship = fromPlnt->ships_res.back();
		fromPlnt->ships_res.pop_back();

		auto callback = CallFunc::create([toPlnt, ship]() {
			toPlnt->ships_res.push_back(ship);
		});
		auto moveTo = MoveTo::create(time, Vec2(toPlnt->posx, toPlnt->posy));

		auto seq = Sequence::create(moveTo, callback, nullptr);
		ship->runAction(seq);

		/*auto moveTo = MoveTo::create(time, Vec2(toPlnt->posx, toPlnt->posy));
		
		Sequence* seq;

		if (toPlnt->type == 4) {
			auto callback1 = CallFunc::create([this, toPlnt]() {
				this->checkAst(toPlnt);
			});
			auto callback2 = CallFunc::create([this]() {
				this->checkRes();
			});
			seq = Sequence::create(moveTo, callback1, callback2, nullptr);

			
		} else {
			auto callback2 = CallFunc::create([this]() {
				this->checkRes();
			});
			seq = Sequence::create(moveTo, callback2, nullptr);
		}
		
		ship->runAction(seq);*/
	}
}

template<class F>
void Empire::checkAtt(F* plnt, int n) {
	if (plnt->emp != this) {
		if (plnt->emp != nullptr && plnt->emp->ai != nullptr) {
			plnt->emp->ai->updateAtt(plnt);
		}
		int enemy_hp = 0;
		int enemy_att = 0;
		Empire* enemy_emp = nullptr;
		if (plnt->ships_att.size() > 0) {
			for (int i = 0; i != plnt->ships_att.size(); ++i) {
				if (plnt->ships_att[i]->emp != this) {
					enemy_hp += plnt->ships_att[i]->hp;
					enemy_att += plnt->ships_att[i]->att;
					if (enemy_emp == nullptr) {
						enemy_emp = plnt->ships_att[i]->emp;
					}
				}
			}
		}
		if (plnt->ships_res.size() > 0) {
			for (int i = 0; i != plnt->ships_res.size(); ++i) {
				if (plnt->ships_res[i]->emp != this) {
					enemy_hp += plnt->ships_res[i]->hp;
					enemy_att += plnt->ships_res[i]->att;
					if (enemy_emp == nullptr) {
						enemy_emp = plnt->ships_res[i]->emp;
					}
				}
			}
		}
		if (plnt->ships_col.size() > 0) {
			for (int i = 0; i != plnt->ships_col.size(); ++i) {
				if (plnt->ships_col[i]->emp != this) {
					enemy_hp += plnt->ships_col[i]->hp;
					enemy_att += plnt->ships_col[i]->att;
					if (enemy_emp == nullptr) {
						enemy_emp = plnt->ships_col[i]->emp;
					}
				}
			}
		}
		if (enemy_hp > 0 || plnt->emp != nullptr) {
			int delta_hp = n*ATT_HP - enemy_hp - PLANET_ADD_HP;
			int delta_att = n*ATT_ATT - enemy_att - PLANET_ADD_ATT;
			if ((delta_hp + delta_att) > 0) {
				log("------Win------");
				int k = n - round((delta_hp + delta_att) / 110);
				log("Ships deleting after attack: %d", k);
				if (k < 0) {
					k = 0;
				}
				auto callback = CallFunc::create([enemy_emp, this, plnt, k]() {
					enemy_emp->cleanPlanet(plnt);
					this->deleteShips(Empire::ATT, k, plnt);
				});
				auto seq = Sequence::create(DelayTime::create(WAR_TIME), callback, nullptr);
				CCParticleSystemQuad* p = CCParticleSystemQuad::create("fire2.plist");
				p->setPosition(plnt->getPosition());
				p->setScale(0.75f);
				Scene* sc = Director::getInstance()->getRunningScene();
				Main* layer = (Main*)sc->getChildByName("main");
				layer->ltime = utils::getTimeInMilliseconds();
				layer->addChild(p, 3);
				sc->runAction(seq);
				log("------EndWin------");
			}
			else {
				CCParticleSystemQuad* p = CCParticleSystemQuad::create("fire2.plist");
				p->setPosition(plnt->getPosition());
				p->setScale(0.8f);
				Scene* sc = Director::getInstance()->getRunningScene();
				Main* layer = (Main*)sc->getChildByName("main");
				layer->ltime = utils::getTimeInMilliseconds();
				layer->addChild(p, 3);
				int k = round(-(delta_hp + delta_att) / 110);
				auto callback = CallFunc::create([enemy_emp, this, plnt, k]() {
					this->cleanPlanet(plnt);
					enemy_emp->deleteShipsAfterAttack(k, plnt);
				});
				auto seq = Sequence::create(DelayTime::create(WAR_TIME), callback, nullptr);
				sc->runAction(seq);
			}
		}
	}
}

template<class F>
void Empire::deleteShipsAfterAttack(int n, F* plnt) {
	while ((plnt->ships_col.size()+ plnt->ships_res.size()+ plnt->ships_att.size())!=n){
		if (plnt->ships_col.size() > 0) {
			this->deleteShip(Empire::COL, plnt);
		}
		else if (plnt->ships_res.size() > 0) {
			this->deleteShip(Empire::RES, plnt);
		}
		else {
			this->deleteShip(Empire::ATT, plnt);
		}
	}
	plnt->setColor(this->color);
}

void Empire::cleanPlanet(Asteroid* plnt) {

	int a, r, c;
	a = r = c = 0;
	log("Deleting ships from ast %d %d %d", plnt->ships_att.size(), plnt->ships_res.size(), plnt->ships_col.size());
	for (int i = 0; i < plnt->ships_att.size(); ++i) {
		if (plnt->ships_att[i]->emp == this) {
			a++;
		}
	}
	for (int i = 0; i < plnt->ships_res.size(); ++i) {
		if (plnt->ships_res[i]->emp == this) {
			r++;
		}
	}
	for (int i = 0; i < plnt->ships_col.size(); ++i) {
		if (plnt->ships_col[i]->emp == this) {
			c++;
		}
	}
	this->deleteShips(Empire::ATT, a, plnt);
	this->deleteShips(Empire::RES, r, plnt);
	this->deleteShips(Empire::COL, c, plnt);
	log("Deleting ships from ast %d %d %d", a, r, c);
	try {
		this->asteroids.erase(std::remove(this->asteroids.begin(), this->asteroids.end(), plnt), this->asteroids.end());
	} catch(...) {

	}
}
void Empire::cleanPlanet(Planet* plnt) {
	int a, r, c;
	a = r = c = 0;
	for (int i = 0; i < plnt->ships_att.size(); ++i) { 
		if (plnt->ships_att[i]->emp == this) {
			a++;
		}
	}
	for (int i = 0; i < plnt->ships_res.size(); ++i) {
		if (plnt->ships_res[i]->emp == this) {
			r++;
		}
	}
	for (int i = 0; i < plnt->ships_col.size(); ++i) {
		if (plnt->ships_col[i]->emp == this) {
			c++;
		}
	}
	this->deleteShips(Empire::ATT, a, plnt);
	this->deleteShips(Empire::RES, r, plnt);
	this->deleteShips(Empire::COL, c, plnt);
	log("Deleting ships %d %d %d", a,r,c);
	plnt->setColor(Color3B(255,255,255));
	try {
		if (this != nullptr) {
			this->planets.erase(std::remove(this->planets.begin(), this->planets.end(), plnt), this->planets.end());
		}
	}
	catch (...) {

	}
	this->checkLose();
	plnt->emp = nullptr;
}

template<class F>
void Empire::deleteShips(Empire::shipType t, int n, F* plnt){
	for (int i = 0; i < n; i++) {
		this->deleteShip(t,plnt);
	}
}

template<class F>
void Empire::deleteShip(Empire::shipType t, F* plnt) {
	if (t == Empire::shipType::ATT) {
		Ship* ship = nullptr;
		for (int i = 0; i < plnt->ships_att.size();++i) {
			if (plnt->ships_att[i]->emp == this) {
				ship = plnt->ships_att[i];
				plnt->ships_att.erase(plnt->ships_att.begin()+i);
				break;
			}
			
		}
		//log("%d", this->ships_att.size());
		this->ships_att.erase(std::remove(this->ships_att.begin(), this->ships_att.end(), ship), this->ships_att.end());
		if (ship != nullptr) {
			ship->removeFromParentAndCleanup(true);
		}
		//log("%d", this->ships_att.size());
	}
	else if (t == Empire::shipType::COL) {
		ShipCol* ship;
		for (int i = 0; i < plnt->ships_col.size(); ++i) {
			if (plnt->ships_col[i]->emp == this) {
				ship = plnt->ships_col[i];
				plnt->ships_col.erase(plnt->ships_col.begin() + i);
				break;
			}

		}
		log("Was col:%d", this->ships_col.size());
		this->ships_col.erase(std::remove(this->ships_col.begin(), this->ships_col.end(), ship), this->ships_col.end());
		if (ship != nullptr) {
			ship->removeFromParentAndCleanup(true);
		}
		log("Now col:%d", this->ships_col.size());
	}
	else {
		ShipRes* ship;
		for (int i = 0; i < plnt->ships_res.size(); ++i) {
			if (plnt->ships_res[i]->emp == this) {
				ship = plnt->ships_res[i];
				plnt->ships_res.erase(plnt->ships_res.begin() + i);
				break;
			}

		}
		//log("%d", this->ships_res.size());
		this->ships_res.erase(std::remove(this->ships_res.begin(), this->ships_res.end(), ship), this->ships_res.end());
		if (ship != nullptr) {
			ship->removeFromParentAndCleanup(true);
		}
		//log("%d", this->ships_res.size());
	}
}

float Empire::getDistance(cocos2d::Sprite* plnt1, cocos2d::Sprite* plnt2) {
	auto pos1 = plnt1->getPosition();
	auto pos2 = plnt2->getPosition();
	float dist = sqrtf(pow((pos1.x-pos2.x), 2)+pow((pos1.y - pos2.y),2));
	return dist;
}

void Empire::checkCol(Planet* plnt) {
	if (plnt->emp != this) {
		this->planets.push_back(plnt);
		plnt->emp = this;
		plnt->setColor(this->color);
	}
}

template<class F>
void Empire::checkAst(F* ast) {
	if (std::find(this->asteroids.begin(), this->asteroids.end(), ast) == this->asteroids.end()) {
		this->asteroids.push_back(ast);
	}
}

template<>
void Empire::checkAst(Planet* ast) {}

void Empire::checkRes() {
	this->diam_delta = 0;
	this->ener_delta = 0;
	this->powd_delta = 0;
	for (int i = 0; i<this->planets.size(); i++) {
		int n = this->planets[i]->ships_res.size();

		this->diam_delta += round((1 + 0.1*n)*PLANET_DIAM);
		this->ener_delta += round((1 + 0.1*n)*PLANET_ENER);
		this->powd_delta += round((1 + 0.1*n)*PLANET_POWD);
	}
	for (int i = 0; i<this->asteroids.size(); i++) {
		int n = this->asteroids[i]->ships_res.size();

		this->diam_delta += round((1 + 0.1*n)* 2 * PLANET_DIAM);
		this->ener_delta += round((1 + 0.1*n)* 2 * PLANET_ENER);
		this->powd_delta += round((1 + 0.1*n)* 2 * PLANET_POWD);
	}
}

void Empire::buyShips(int att, int res, int col, Planet* plnt) {
	auto pos = plnt->getPosition();
	this->createShips(Empire::shipType::ATT, att, pos.x , pos.y ,plnt);
	this->createShips(Empire::shipType::RES, res, pos.x, pos.y, plnt);
	this->createShips(Empire::shipType::COL, col, pos.x, pos.y, plnt);
	if (res>0) {
		this->checkRes();
	}
	int d = att*ATT_DIAM + res*RES_DIAM + col*COL_DIAM;
	int e = att*ATT_ENER + res*RES_ENER + col*COL_ENER;
	int p = att*ATT_POWD + res*RES_POWD + col*COL_POWD;

	this->descRes(d, e, p);
}

void Empire::descRes(int d, int e, int p) {
	this->diam -= d;
	this->ener -= e;
	this->powd -= p;
}

Planet* Empire::getNearestPlanet() {
	Main* main = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	float min = 1280.0f;
	Planet* minPl = nullptr;
	for (int i = 0; i < this->planets.size(); ++i) {
		for (int j = 0; j < main->planets.size(); ++j) {
			if (main->planets[j]->emp != nullptr) {
				continue;
			} else {
				float d = this->getDistance(this->planets[i], main->planets[j]);
				if(d<min){
					min = d;
					minPl = main->planets[j];
					this->fromNear = this->planets[i];
				}
			}
		}
	}
	return minPl;
}

Asteroid* Empire::getNearestAsteroid() {
	Main* main = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	float min = 1280.0f;
	Asteroid* minPl = nullptr;
	for (int i = 0; i < this->planets.size(); ++i) {
		for (int j = 0; j < main->asteroids.size(); ++j) {
			if (main->asteroids[j]->emp != nullptr) {
				continue;
			}
			else {
				float d = this->getDistance(this->planets[i], main->asteroids[j]);
				if (d<min) {
					min = d;
					minPl = main->asteroids[j];
					this->fromNear = this->planets[i];
				}
			}
		}
	}
	return minPl;
}

Empire* Empire::getNearestEmpire() {
	Main* main = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	float min = 1280.0f;
	Empire* emp = nullptr;
	for (int i = 0; i < main->empires.size(); ++i) {
		if (main->empires[i] == this) {
			continue;
		} else {
			float d = this->getDistance(this, main->empires[i]);
			if(d < min) {
				min = d;
				emp = main->empires[i];
			}
		}
	}
	return emp;
}

float Empire::getDistance(Empire* emp1, Empire* emp2) {
	float min = 1280.0f;
	for (int i = 0; i < emp1->planets.size(); ++i) {
		for (int j = 0; j < emp2->planets.size(); ++j) {
			float d = this->getDistance(emp1->planets[i], emp2->planets[j]);
			if (d<min) {
				min = d;
			}
		}
	}
	return min;
}

int Empire::getPlanetRating(Planet* plnt) {
	int plnt_hp = PLANET_ADD_HP;
	int plnt_att = PLANET_ADD_ATT;
	if (plnt->ships_att.size() > 0) {
		for (int i = 0; i != plnt->ships_att.size(); ++i) {
			plnt_hp += plnt->ships_att[i]->hp;
			plnt_att += plnt->ships_att[i]->att;
		}
	}
	if (plnt->ships_res.size() > 0) {
		for (int i = 0; i != plnt->ships_res.size(); ++i) {
			plnt_hp += plnt->ships_res[i]->hp;
			plnt_att += plnt->ships_res[i]->att;
		}
	}
	if (plnt->ships_col.size() > 0) {
		for (int i = 0; i != plnt->ships_col.size(); ++i) {
			plnt_hp += plnt->ships_col[i]->hp;
			plnt_att += plnt->ships_col[i]->att;
		}
	}
	return plnt_att + plnt_hp;
}

void Empire::checkLose() {
	if (this->planets.empty()) {
		Main* layer = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
		layer->clearEmp(this);
	}
}