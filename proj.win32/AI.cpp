#include "MainScene.h"
#include "AI.h"
using namespace cocos2d;

AI::AI(Empire* emp) {
	this->emp = emp;
}

bool AI::init() {
	return true;
}

AI* AI::create(Empire* emp) {
	AI* ai = new AI(emp);
	if (ai && ai->init())
	{
		return ai;
	}
	return nullptr;
}
void AI::updateAtt(Asteroid* node) {}
void AI::updateAtt(Planet* node) {
	/*this->state = AI::VEN;
	this->vendeta = node;
	this->update();*/
}

void AI::update() {
	this->checkState();
	this->doState();
}

void AI::checkState() {
	if (this->state == AI::VEN) {

	}else if (this->emp->planets.size()==1) {
		this->state = AI::DEF;
	} else if (this->getTopEmp() == this->emp || this->attPlnt != nullptr) {
		this->state = AI::ATT;
	} else {
		this->state = AI::RES;
	}
}

void AI::doState() {
	switch(this->state) {
	case AI::RES:{
		Asteroid* ast = this->emp->getNearestAsteroid();
		if (!ast->ships_res.empty()) {
			Planet * plnt = this->emp->getNearestPlanet();
			int n = this->countBuyShips(1);
			if (n > 0) {
				this->emp->buyShips(0, n, 0, this->emp->fromNear);
				this->emp->moveShips(Empire::RES, n, this->emp->fromNear, plnt);
				this->colPlanet(plnt);
			}
		} else {
			int n = this->countBuyShips(1);
			if (n > 0) {
				this->emp->buyShips(0, n, 0, this->emp->fromNear);
				this->emp->moveShips(Empire::RES, n, this->emp->fromNear, ast);
			}
		}
		break;
	}
	case AI::DEF:{
		if (this->emp->planets.size() == 1) {
			Planet* plnt = this->emp->planets.front();
			if (this->emp->getPlanetRating(plnt)>AI_DEF_ONE_COL) {
				int n = this->countBuyShips(2);
				if (n > 0) {
					this->emp->buyShips(0, 0, 1, plnt);
					Planet* colony = this->emp->getNearestPlanet();
					if (colony != nullptr) {
						this->emp->moveShips(Empire::COL, 1, plnt, colony);
					}
				} else {
					/*int n = this->countBuyShips(1);
					if (n > 0 && this->emp->ships_col.size()<5) {
						this->emp->buyShips(0, 1, 0, plnt);
					}*/
				}
			} else {
				int n = this->countBuyShips(1);
				if (n > 1) {
					this->emp->buyShips(0, 1, 0, plnt);
				}
				n = this->countBuyShips(0);
				if (n > 0) {
					this->emp->buyShips(n, 0, 0, plnt);
				}
			}
		}
		break;
	}
	case AI::ATT:{
		if (this->enemy == nullptr) {
			this->enemy = this->emp->getNearestEmpire();
		}
		Planet* plnt;
		try {
			plnt = this->getNearestPlanet(this->enemy);
		} catch (const std::exception&) {
			plnt = nullptr;
		}
		
		if (this->attPlnt == nullptr && plnt!=nullptr) {
			int rate = this->emp->getPlanetRating(plnt);
			int att_rate = this->getAttPotential();
			log("rate %d < att potential %d", rate, att_rate);
			if (att_rate > rate) {
				int n = rate / (ATT_HP + ATT_ATT) + 1;
				log("prepAtt %d", n);
				float t = this->prepAtt(this->emp->fromNear, n);
				log("att time %f", t);
				if (t != 0.0f) {
					auto callback = CallFunc::create([this, plnt, n]() {
						this->emp->moveShips(Empire::ATT, n, this->emp->fromNear, plnt);
					});
					auto seq = Sequence::create(DelayTime::create(t), callback, nullptr);
					Director::getInstance()->getRunningScene()->runAction(seq);
					this->attPlnt = plnt;
				}
			}
		} else {
			if (this->attPlnt->emp == nullptr) {
				this->colPlanet(this->attPlnt);
				this->attPlnt = nullptr;
				this->enemy = nullptr;
			}
		}
		break;
	}
	case AI::VEN: {
		if (this->enemy == nullptr) {
			this->enemy = this->vendeta->emp;
		}

		if (this->vendeta != nullptr) {
			int rate = this->emp->getPlanetRating(this->vendeta);
			int att_rate = this->getAttPotential();
			log("rate %d < att potential %d", rate, att_rate);
			if (att_rate > rate) {
				int n = rate / (ATT_HP + ATT_ATT) + 1;
				log("prepAtt %d", n);
				float t = this->prepAtt(this->emp->planets.front(), n);
				log("att time %f", t);
				if (t != 0.0f) {
					auto callback = CallFunc::create([this, n]() {
						this->emp->moveShips(Empire::ATT, n, this->emp->fromNear, this->vendeta);
					});
					auto seq = Sequence::create(DelayTime::create(t), callback, nullptr);
					Director::getInstance()->getRunningScene()->runAction(seq);
				}
			} else {
				this->state = AI::DEF;
				this->doState();
				this->vendeta = nullptr;
				this->enemy = nullptr;
			}
		}
	}
	default:
		break;
	}
}

bool AI::colPlanet(Planet* plnt) {
	if (this->emp->ships_col.size() > 0) {
		for (unsigned i = 0; i < this->emp->planets.size(); i++) {
			if (this->emp->planets[i]->ships_col.size()>0) {
				this->emp->moveShips(Empire::COL, 1, this->emp->planets[i], plnt);
				return true;
			}
		}
	} else {
		int n = this->countBuyShips(2);
		if (n > 0) {
			if (this->emp->fromNear != nullptr) {
				this->emp->buyShips(0, 0, 1, this->emp->fromNear);
				this->emp->moveShips(Empire::COL, 1, this->emp->fromNear, plnt);
			} else {
				return false;
			}
		} else {
			return false;
		}
	}
}

Empire* AI::getTopEmp() {
	Main* layer = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	return layer->topEmpire;
}

float AI::prepAtt(Planet* plnt,int n) {
	float maxTime = 0.0f; 
	int more = n - plnt->ships_att.size();
	if(more<0){
		return 0.01f;
	}
	for (std::vector<int>::size_type i = 0; i < this->emp->planets.size(); i++) {
		if (more == 0) {
			break;
		}
		int sz = this->emp->planets[i]->ships_att.size();
		if (this->emp->planets[i]!=plnt && sz>0) {
			float dist = this->emp->getDistance(plnt, this->emp->planets[i]);
			float time = dist / SHIP_SPEED;
			if (time > maxTime) {
				maxTime = time;
			}
			if (sz > more) {
				this->emp->moveShips(Empire::ATT,more, this->emp->planets[i], plnt);
				more = 0;
			} else {
				this->emp->moveShips(Empire::ATT, sz, this->emp->planets[i], plnt);
				more -= sz;
			}
		}
	}
	if (more > 0) {
		if(this->countBuyShips(0) >= more) {
			this->emp->buyShips(more, 0, 0, plnt);
		}
		else {
			maxTime = 0.0f;
		}
	}
	return maxTime;
}

int AI::getAttPotential() {
	int r = this->emp->ships_att.size()*ATT_ATT + this->emp->ships_att.size()*ATT_HP;
	r += this->countBuyShips(0)*ATT_ATT + this->countBuyShips(0)*ATT_HP;
	return r;
}

Planet* AI::getNearestPlanet(Empire* emp) {
	float min = 1280.0f;
	Planet* minPl = nullptr;
	if (this->emp->planets.empty() == false && emp->planets.empty() == false) {
		for (std::vector<int>::size_type i = 0; i != this->emp->planets.size(); ++i) {
			for (std::vector<int>::size_type j = 0; j != emp->planets.size(); ++j) {
				float d = this->emp->getDistance(this->emp->planets[i], emp->planets[j]);
				if (d < min) {
					min = d;
					minPl = emp->planets[j];
					this->emp->fromNear = this->emp->planets[i];
				}
			}
		}
	}
	return minPl;
}
int AI::countBuyShips(int t) {
	int d = this->emp->diam;
	int e = this->emp->ener;
	int p = this->emp->powd;
	int r = -1;
	switch (t) {
	case 0:
	{
		while (d > 0 && e > 0 && p > 0) {
			r += 1;
			d -= ATT_DIAM;
			e -= ATT_ENER;
			p -= ATT_POWD;
		}
		break;
	}
	case 1:
	{
		while (d > 0 && e > 0 && p > 0) {
			r += 1;
			d -= RES_DIAM;
			e -= RES_ENER;
			p -= RES_POWD;
		}
		break;
	}
	case 2:
	{
		while (d > 0 && e > 0 && p > 0) {
			r += 1;
			d -= COL_DIAM;
			e -= COL_ENER;
			p -= COL_POWD;
		}
		break;
	}
	}
	return r;
}