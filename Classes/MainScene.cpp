#include "MainScene.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"


#include "Planet.h"
#include "Asteroid.h"
#include "Definitions.h"
#include "WinScene.h"
#include <math.h>

USING_NS_CC;

USING_NS_CC_EXT;
using namespace ui;

Main* Main::create(int planetNum, int empireNum, bool sim)
{
	Main* widget = new Main();
	if (widget && widget->init())
	{
		widget->sim = sim;
		widget->planetNum = planetNum;
		widget->empireNum = empireNum;
		widget->autorelease();
		widget->addEvents();
		widget->initOptions();
		widget->initPopUp();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

cocos2d::LayerColor* Main::initPopUp() {
	auto popUp = LayerColor::create(Color4B(255,255,255,100));
	popUp->setContentSize(Size(WINDOW_WIDTH, WINDOW_HEIGHT));
	popUp->setSwallowsTouches(true);

	auto labelPr = Label::createWithTTF("Overall price: 0 0 0", "fonts/Marker Felt.ttf", 28);
	labelPr->setPosition(Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 200));
	labelPr->setName("price");
	popUp->addChild(labelPr);

	auto labelAtt = Label::createWithTTF("Number of attacking ships: 0", "fonts/Marker Felt.ttf", 28);
	labelAtt->setPosition(Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 150));
	labelAtt->setName("latt");
	popUp->addChild(labelAtt);

	auto labelRes = Label::createWithTTF("Number of resourse ships: 0", "fonts/Marker Felt.ttf", 28);
	labelRes->setPosition(Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 + 50));
	labelRes->setName("lres");
	popUp->addChild(labelRes);

	auto labelCol = Label::createWithTTF("Number of colonization ships: 0", "fonts/Marker Felt.ttf", 28);
	labelCol->setPosition(Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 50));
	labelCol->setName("lcol");
	popUp->addChild(labelCol);

	Main* main = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");

	auto sliderAtt = ui::Slider::create();
	sliderAtt->loadBarTexture("Default/Slider_Back.png");
	sliderAtt->loadSlidBallTextures("Default/SliderNode_Normal.png", "Default/SliderNode_Press.png", "");
	sliderAtt->loadProgressBarTexture("Default/Slider_PressBar.png");
	sliderAtt->setScale9Enabled(true);
	sliderAtt->setName("satt");
	sliderAtt->setMaxPercent(40);
	sliderAtt->setCapInsets(Rect(0, 0, 0, 0));
	sliderAtt->setContentSize(Size(225.0f, 19));
	sliderAtt->setPosition(Vec2(WINDOW_WIDTH/2, WINDOW_HEIGHT/2+100));
	sliderAtt->addEventListener(CC_CALLBACK_1(Main::sliderAttEvent, main));
	popUp->addChild(sliderAtt, 0);

	auto sliderRes = ui::Slider::create();
	sliderRes->loadBarTexture("Default/Slider_Back.png");
	sliderRes->loadSlidBallTextures("Default/SliderNode_Normal.png", "Default/SliderNode_Press.png", "");
	sliderRes->loadProgressBarTexture("Default/Slider_PressBar.png");
	sliderRes->setScale9Enabled(true);
	sliderRes->setName("sres");
	sliderRes->setMaxPercent(20);
	sliderRes->setCapInsets(Rect(0, 0, 0, 0));
	sliderRes->setContentSize(Size(225.0f, 19));
	sliderRes->setPosition(Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 ));
	sliderRes->addEventListener(CC_CALLBACK_1(Main::sliderResEvent, main));
	popUp->addChild(sliderRes, 0);

	auto sliderCol = ui::Slider::create();
	sliderCol->loadBarTexture("Default/Slider_Back.png");
	sliderCol->loadSlidBallTextures("Default/SliderNode_Normal.png", "Default/SliderNode_Press.png", "");
	sliderCol->loadProgressBarTexture("Default/Slider_PressBar.png");
	sliderCol->setScale9Enabled(true);
	sliderCol->setName("scol");
	sliderCol->setMaxPercent(10);
	sliderCol->setCapInsets(Rect(0, 0, 0, 0));
	sliderCol->setContentSize(Size(225.0f, 19));
	sliderCol->setPosition(Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 100));
	sliderCol->addEventListener(CC_CALLBACK_1(Main::sliderColEvent, main));
	popUp->addChild(sliderCol, 0);

	

	auto btn = ui::Button::create("btn_buy.png", "btn_buy.png", "trans.png");
	btn->setPosition(Vec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2 - 200));
	btn->setScale(1.0f);
	btn->setName("buybtn");
	btn->setEnabled(true);
	btn->addTouchEventListener(CC_CALLBACK_0(Main::popbtnPressed, main));
	popUp->addChild(btn);

	return popUp;
}

void Main::showPopUp(Planet* plnt) {
	this->buy_plnt = plnt;
	auto popUp = (cocos2d::Layer*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(0);
	popUp->setLocalZOrder(2);
}

void Main::hidePopUp() {
	auto popUp = (cocos2d::Layer*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(0);
	if (popUp->getLocalZOrder() == 2) {
		cocos2d::ui::Slider* slideratt = (cocos2d::ui::Slider*)popUp->getChildByName("satt");
		int natt = slideratt->getPercent();
		cocos2d::ui::Slider* sliderres = (cocos2d::ui::Slider*)popUp->getChildByName("sres");
		int nres = sliderres->getPercent();
		cocos2d::ui::Slider* slidercol = (cocos2d::ui::Slider*)popUp->getChildByName("scol");
		int ncol = slidercol->getPercent();

		Main* main = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");

		main->playerEmpire->buyShips(natt, nres, ncol, main->buy_plnt);

		main->updateMenu();

		popUp->setLocalZOrder(-1);
	}
}

Scene* Main::createScene(int planetNum, int empireNum, bool sim)
{
    auto scene = Scene::create();
    auto layer = Main::create(planetNum, empireNum, sim);
	auto popUp = Main::initPopUp();
	cocos2d::Layer* mlayer = Main::createMenu(layer);
	scene->addChild(layer,0);
	scene->addChild(mlayer,1);
	scene->addChild(popUp, -1, 0);
	layer->updateEmpires(0.0f);
    return scene;
}

cocos2d::Layer* Main::createMenu(Main* layer) {
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	layer->menu = Layer::create();

	auto label_d = Label::createWithTTF("Diamonds: "+ std::to_string(START_DIAM), "fonts/Marker Felt.ttf", 21);
	label_d->setName("label_d");
	label_d->setPosition(Vec2(visibleSize.width / 2 + origin.x - 200, visibleSize.height - origin.y-50));
	layer->menu->addChild(label_d,0);

	auto label_e = Label::createWithTTF("Energy Crystals: " + std::to_string(START_ENER), "fonts/Marker Felt.ttf", 21);
	label_e->setName("label_e");
	label_e->setPosition(Vec2(visibleSize.width / 2 + origin.x , visibleSize.height - origin.y - 50));
	layer->menu->addChild(label_e, 0);

	auto label_p = Label::createWithTTF("Powder: " + std::to_string(START_POWD), "fonts/Marker Felt.ttf", 21);
	label_p->setName("label_p");
	label_p->setPosition(Vec2(visibleSize.width / 2 + origin.x+ 200, visibleSize.height - origin.y - 50));
	layer->menu->addChild(label_p, 0);

	layer->shipState = ui::Slider::create();
	layer->shipState->loadBarTexture("Default/Slider_Back.png");
	layer->shipState->loadSlidBallTextures("Default/SliderNode_Normal.png", "Default/SliderNode_Press.png", "");
	layer->shipState->loadProgressBarTexture("Default/Slider_PressBar.png");
	layer->shipState->setScale9Enabled(true);
	layer->shipState->setMaxPercent(2);
	layer->shipState->setCapInsets(Rect(0, 0, 0, 0));
	layer->shipState->setContentSize(Size(125.0f, 19));
	layer->shipState->setPosition(Vec2(origin.x+ 100 , 50));
	layer->menu->addChild(layer->shipState, 0);
	//this->shipState->addEventListener(CC_CALLBACK_1(PlayMenuScene::empireSliderEvent, this));


	return layer->menu;
}

bool Main::init()
{
	if (Layout::init())
	{
		setClippingEnabled(true);
		_innerContainer->setTouchEnabled(false);
		if (_scrollBarEnabled)
		{
			initScrollBar();
		}
		return true;
	}
	return false;
}

void Main::generateGame() {
	this->generateMap();
	this->generateEmpires();
}

void Main::generateEmpires() {
	for (int i = 0; i < this->empireNum; i++) {
		int planetN = i*(int)(this->planets.size()/this->empireNum);
		auto emp = Empire::create(this, this->colorSwatches[i]);
		this->planets[planetN]->emp = emp;
		if (i == 0 && this->sim==false) {
			this->playerEmpire = emp;
			emp->player = true;
			//this->planets[planetN]->addEventsPlayer();
		}
		else {
			if (_AI_) {
				emp->createAI();
			}
		}
		emp->setStartingPlanet(this->planets[planetN]);
		this->empires.push_back(emp);
	}
	this->schedule(schedule_selector(Main::updateEmpires),5.0f);
}

void Main::updateTopEmpire() {
	int k = 0; int s; int max = 0;
	log("up");
	//log("%d", this->empires[0]->ships_res.size());
	//log("%d", this->empires[0]->ships_col.size());
	for (std::vector<int>::size_type i = 0; i < this->empires.size(); ++i) {
		s = 0;
		if (this->empires[i] == nullptr) {
			throw empireGetExeption();
		}
		if (!this->empires[i]->ships_att.empty()) {
			s = this->empires[i]->ships_att.size()*(ATT_DIAM + ATT_ENER + ATT_POWD);
		}
		s += this->empires[i]->ships_col.size()*(COL_DIAM + COL_ENER + COL_POWD);
		s += this->empires[i]->ships_res.size()*(RES_DIAM + RES_ENER + RES_POWD);
		s += this->empires[i]->diam;
		s += this->empires[i]->ener;
		s += this->empires[i]->powd;
		if (s > max) {
			max = s; k = i;
		}
	}
	this->topEmpire = this->empires[k];
	log("Top empire: %d",k);
}

void Main::updateEmpires(float delta) {
	try {
		this->updateTopEmpire();
	}
	catch (const empireGetExeption& e) {
		log(e.what());
	}
	for (int i = 0; i < this->empires.size(); i++) {
		this->empires[i]->diam += this->empires[i]->diam_delta;
		this->empires[i]->ener += this->empires[i]->ener_delta;
		this->empires[i]->powd += this->empires[i]->powd_delta;
		if (this->empires[i]->player == false && (_AI_)) {
			try {
				this->empires[i]->ai->update();
			}
			catch (...) {
				log("AI Error");
			}
		}
	}
	if (this->sim == false) {
		this->updateMenu();
	}
}

void Main::updateMenu() {
	Label* l_d = (Label *)this->menu->getChildByName("label_d");
	int diam = this->playerEmpire->diam;
	std::string label = "Diamonds: " + std::to_string(diam);
	l_d->setString(label);

	Label* l_e = (Label *)this->menu->getChildByName("label_e");
	int ener = this->playerEmpire->ener;
	label = "Energy crystals: " + std::to_string(ener);
	l_e->setString(label);

	Label* l_p = (Label *)this->menu->getChildByName("label_p");
	int powd = this->playerEmpire->powd;
	label = "Powder: " + std::to_string(powd);
	l_p->setString(label);
}

void Main::genPoints(int minX, int maxX, int minY, int maxY, int points) {
	if (points == 0) {

	} else if (points > 2) {
		int pointsHalf = (int)(points / 2.0 + 0.5);
		int pointsOne = (int)(pointsHalf / 2.0 + 0.5);
		int pointsTwo = pointsHalf - pointsOne;
		int pointsThree = (int)((points - pointsHalf) / 2.0 + 0.5);
		int pointsFour = points - pointsHalf - pointsThree;
		this->genPoints(minX, (maxX+minX) / 2, minY, (maxY+minY) / 2, pointsFour);
		this->genPoints(minX, (maxX + minX) / 2, (maxY + minY) / 2, maxY, pointsThree);
		this->genPoints((maxX + minX) / 2, maxX, minY, (maxY + minY) / 2, pointsTwo);
		this->genPoints((maxX + minX) / 2, maxX, (maxY + minY) / 2, maxY, pointsOne);
	}
	else {
		int offset = PLANET_RADIUS+PLANET_RADIUS/2;
		int x = minX + offset + rand() % (maxX - minX - offset * 2);
		int y = minY + offset + rand() % (maxY - minY - offset * 2);
		this->xAxis.push_back(x);
		this->yAxis.push_back(y);
		int i = 1;
		bool ch;
		while (i < points) {
			int x = minX + offset + rand() % (maxX - minX - offset * 2);
			int y = minY + offset + rand() % (maxY - minY - offset * 2);
			ch = true;
			for (int j = 0; j < xAxis.size(); j++) {
				double distance = sqrt(pow(xAxis[j] - x, 2) + pow(yAxis[j] - y, 2));
				if (distance <= offset*2) ch = false;
			}
			if (ch) {
				this->xAxis.push_back(x);
				this->yAxis.push_back(y);
				i++;
			}
		}
	}
}

void Main::generateMap() {
	this->genPoints(0, PLAYSCENE_WIDTH, 0, PLAYSCENE_HEIGHT, this->planetNum);

	for (int i = 0; i < this->planetNum; i++) {
		if (i % 4 == 0) {
			Asteroid* ast = Asteroid::create(this->xAxis[i], this->yAxis[i]);
			this->asteroids.push_back(ast);
			this->addChild(ast);
		}
		else {
			Planet* planet = Planet::create(this->xAxis[i], this->yAxis[i]);
			this->planets.push_back(planet);
			this->addChild(planet);
		}
	}
	/*for (int i = 0; i < this->planets.size(); i++) {
		auto pos = planets.at(i)->getPosition();
		log("%f %f\n", pos.x, pos.y);
	}*/
}

bool Main::initOptions() {	
	this->setSize(Size(WINDOW_WIDTH, WINDOW_HEIGHT));
	this->setInnerContainerSize(Size(PLAYSCENE_WIDTH, PLAYSCENE_HEIGHT));
	this->setInnerContainerPosition(Vec2(0,0));
	this->setDirection(Direction::BOTH);
	this->removeScrollBar();
	this->setBounceEnabled(true);
	this->setName("main");
	this->setBackGroundColorType(BackGroundColorType::SOLID);
	this->setBackGroundColor(Color3B(0,0,0));
  
	srand(time(NULL));

	this->generateGame();

    return true;
}

void Main::addEvents(){
	auto listener = cocos2d::EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(Main::onKeyPressed, this);

	cocos2d::Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

Scene* Main::loadScene(int n) {
	auto scene = Scene::create();
	auto layer = Main::load(n);
	auto popUp = Main::initPopUp();
	cocos2d::Layer* mlayer = Main::createMenu(layer);
	scene->addChild(layer, 0);
	scene->addChild(mlayer, 1);
	scene->addChild(popUp, -1, 0);
	layer->updateMenu();
	return scene;
}

Main* Main::load(int tag) {
	Main* widget = new Main();
	if (widget && widget->init())
	{
		widget->autorelease();
		widget->addEvents();
		widget->initOptionsLoad(tag);
		widget->initPopUp();
		return widget;
	}
	CC_SAFE_DELETE(widget);
	return nullptr;
}

bool Main::initOptionsLoad(int n) {
	this->setSize(Size(WINDOW_WIDTH, WINDOW_HEIGHT));
	this->setInnerContainerSize(Size(PLAYSCENE_WIDTH, PLAYSCENE_HEIGHT));
	this->setInnerContainerPosition(Vec2(0, 0));
	this->setDirection(Direction::BOTH);
	this->removeScrollBar();
	this->setBounceEnabled(true);
	this->setName("main");
	this->setBackGroundColorType(BackGroundColorType::SOLID);
	this->setBackGroundColor(Color3B(0, 0, 0));
	srand(time(NULL));

	try {
		this->loadGame(n);
	}
	catch (const fileOpenFail& e) {
		log(e.what());
	}
	catch (const planetCreateFail& e) {
		log(e.what());
	}
	catch (const asteroidCreateFail& e) {
		log(e.what());
	}
	catch (const empireCreateFail& e) {
		log(e.what());
	}
	catch (const addNodeFail& e) {
		log(e.what());
	}
	catch (...) {
		log("Uknown exception thrown");
	}
	this->schedule(schedule_selector(Main::updateEmpires), 5.0f);
	return true;
}

void Main::loadGame(int n) {
	std::fstream c("C:/Users/adams/Desktop/saves/save" + std::to_string(n) + ".txt", std::ios_base::in);
	if (!c)
		throw fileOpenFail();
	int num, num_in, par1, par2, par3, par4, par5, par6, i, j;
	c >> num;
	for (i = 0; i < num; ++i) {
		c >> par1 >> par2 >> par3;
		Planet* planet = Planet::create(par3, par1, par2);
		if (planet == nullptr) {
			throw planetCreateFail();
		}
		this->planets.push_back(planet);
		this->addChild(planet);
	}
	c >> num;
	for (i = 0; i < num; ++i) {
		c >> par1 >> par2 >> par3;
		Asteroid* ast = Asteroid::create(par1, par2);
		if (ast == nullptr) {
			throw asteroidCreateFail();
		}
		this->asteroids.push_back(ast);
		this->addChild(ast);
	}
	c >> num;
	for (j = 0; j < num; ++j) {
		c >> par1 >> par2 >> par3 >> par4 >> par5 >> par6;
		Empire* emp = Empire::create(this, Color3B(par1,par2,par3));
		if (emp == nullptr) {
			throw empireCreateFail();
		}
		if (j == 0) {
			emp->player = true;
			this->playerEmpire = emp;
		}
		emp->diam = par4;
		emp->ener = par5;
		emp->powd = par6;
		this->empires.push_back(emp);
		c >> num_in;
		for (i = 0; i < num_in; ++i) {
			c >> par1 >> par2 >> par3;
			if (!this->addNode(emp,par1, par2)) {
				throw addNodeFail();
			}
		}
		c >> num_in;
		for (i = 0; i < num_in; ++i) {
			c >> par1 >> par2 >> par3;
			if (!this->addNode(emp, par1, par2)) {
				throw addNodeFail();
			}
		}
		c >> num_in;
		for (i = 0; i < num_in; ++i) {
			c >> par1 >> par2 >> par3;
			Ship* ship = Ship::create(par1, par2, par3);
			ship->emp = emp;
			emp->ships_att.push_back(ship);
			this->addShipToNode(ship, par1,par2);
			this->addChild(ship, 0);
		}
		c >> num_in;
		for (i = 0; i < num_in; ++i) {
			c >> par1 >> par2 >> par3;
			ShipRes* ship = ShipRes::create(par1, par2, par3);
			ship->emp = emp;
			emp->ships_res.push_back(ship);
			this->addResToNode(ship, par1, par2);
			this->addChild(ship, 0);
		}
		c >> num_in;
		for (i = 0; i < num_in; ++i) {
			c >> par1 >> par2 >> par3;
			ShipCol* ship = ShipCol::create(par1, par2, par3);
			ship->emp = emp;
			emp->ships_col.push_back(ship);
			this->addColToNode(ship, par1, par2);
			this->addChild(ship, 0);
		}
		if (j == 0) {
			emp->checkRes();
		}
	}
	c.close();
}

bool Main::addShipToNode(Ship* ship, int posx, int posy) {
	for (std::vector<Planet*>::iterator it = this->planets.begin(); it != this->planets.end(); ++it) {
		auto pos = (*it)->getPosition();
		if (pos.x == posx && pos.y == posy) {
			(*it)->ships_att.push_back(ship);
			return true;
		}
	}
	for (std::vector<Asteroid*>::iterator ita = this->asteroids.begin(); ita != this->asteroids.end(); ++ita) {
		auto pos = (*ita)->getPosition();
		if (pos.x == posx && pos.y == posy) {
			(*ita)->ships_att.push_back(ship);
			return true;
		}
	}
	return false;
}

bool Main::addResToNode(ShipRes* ship, int posx, int posy) {
	for (std::vector<Planet*>::iterator it = this->planets.begin(); it != this->planets.end(); ++it) {
		auto pos = (*it)->getPosition();
		if (pos.x == posx && pos.y == posy) {
			(*it)->ships_res.push_back(ship);
			return true;
		}
	}
	for (std::vector<Asteroid*>::iterator ita = this->asteroids.begin(); ita != this->asteroids.end(); ++ita) {
		auto pos = (*ita)->getPosition();
		if (pos.x == posx && pos.y == posy) {
			(*ita)->ships_res.push_back(ship);
			return true;
		}
	}
	return false;
}

bool Main::addColToNode(ShipCol* ship, int posx, int posy) {
	for (std::vector<Planet*>::iterator it = this->planets.begin(); it != this->planets.end(); ++it) {
		auto pos = (*it)->getPosition();
		if (pos.x == posx && pos.y == posy) {
			(*it)->ships_col.push_back(ship);
			return true;
		}
	}
	for (std::vector<Asteroid*>::iterator ita = this->asteroids.begin(); ita != this->asteroids.end(); ++ita) {
		auto pos = (*ita)->getPosition();
		if (pos.x == posx && pos.y == posy) {
			(*ita)->ships_col.push_back(ship);
			return true;
		}
	}
	return false;
}

bool Main::addNode(Empire* emp, int posx, int posy) {
	for (std::vector<Planet*>::iterator it = this->planets.begin(); it != this->planets.end(); ++it) {
		auto pos = (*it)->getPosition();
		if (pos.x == posx && pos.y == posy) {
			(*it)->emp = emp;
			(*it)->setColor(emp->color);
			emp->planets.push_back((*it));
			return true;
		}
	}
	for (std::vector<Asteroid*>::iterator ita = this->asteroids.begin(); ita != this->asteroids.end(); ++ita) {
		auto pos = (*ita)->getPosition();
		if (pos.x == posx && pos.y == posy) {
			(*ita)->emp = emp;
			emp->asteroids.push_back((*ita));
			return true;
		}
	}
	return false;
}

std::ostream& operator<<(std::ostream& os, const Main* main) {
	os << main->planets.size() << std::endl;
	for (int i = 0; i != main->planets.size(); ++i) {
		os << main->planets[i];
	}
	os << main->asteroids.size() << std::endl;
	for (int i = 0; i != main->asteroids.size(); ++i) {
		os << main->asteroids[i];
	}
	os << main->empires.size() << std::endl;
	for (int i = 0; i != main->empires.size(); ++i) {
		os << main->empires[i];
	}
	return os;
}

void Main::saveGame() {
	log("Saving the game...");

	auto time = utils::getTimeInMilliseconds();

	if ((time - this->ltime)<=int(WAR_TIME*1000)) {
		throw battleRunning();
	}

	std::fstream c("C:/Users/adams/Desktop/saves/count.txt", std::ios_base::in);
	if (!c)
		throw fileOpenFail();
	int n;
	c >> n;
	c.close();

	std::ofstream f("C:/Users/adams/Desktop/saves/save"+std::to_string(n)+".txt");
	if (!f)
		throw fileOpenFail();
	f << this;
	log("Game saved.");
	f.close();

	std::ofstream ofs("C:/Users/adams/Desktop/saves/count.txt", std::ofstream::out | std::ofstream::trunc);
	ofs << ++n;
	ofs.close();
}

void Main::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_S:
		try {
			this->saveGame();
		}
		catch (const fileOpenFail& e) {
			log(e.what());
		}
		catch (const battleRunning& e) {
			log(e.what());
		}
		catch (const staticShip& e) {
			log(e.what());
		}
		break;
	}
}

void Main::popbtnPressed() {
	this->hidePopUp();
}

void Main::clearEmp(Empire* emp){
	this->empires.erase(std::remove(this->empires.begin(), this->empires.end(), emp), this->empires.end());
	this->checkWin();
}

void Main::checkWin() {
	if (this->empires.size() == 1) {
		auto scene = WinScene::createScene(this->empires.front()->color);
		Director::getInstance()->replaceScene(TransitionFade::create(TRANSITION_TIME, scene));
	}
}

void Main::sliderAttEvent(cocos2d::Ref * sender) {
	auto popUp = (cocos2d::Layer*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(0);
	Label* label = (Label*)popUp->getChildByName("latt");
	cocos2d::ui::Slider* slider = (cocos2d::ui::Slider*)popUp->getChildByName("satt");
	int n = slider->getPercent();
	std::string labelstr = "Number of attacking ships: " + std::to_string(n);
	label->setString(labelstr);

	Main* main = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	main->pr_att = n;
	main->update_price(popUp);
}
void Main::sliderResEvent(cocos2d::Ref * sender) {
	auto popUp = (cocos2d::Layer*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(0);
	Label* label = (Label*)popUp->getChildByName("lres");
	cocos2d::ui::Slider* slider = (cocos2d::ui::Slider*)popUp->getChildByName("sres");
	int n = slider->getPercent();
	std::string labelstr = "Number of resourse ships: " + std::to_string(n);
	label->setString(labelstr);

	Main* main = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	main->pr_res = n;
	main->update_price(popUp);
}
void Main::sliderColEvent(cocos2d::Ref * sender) {
	auto popUp = (cocos2d::Layer*)cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(0);
	Label* label = (Label*)popUp->getChildByName("lcol");
	cocos2d::ui::Slider* slider = (cocos2d::ui::Slider*)popUp->getChildByName("scol");
	int n = slider->getPercent();
	std::string labelstr = "Number of colonization ships: " + std::to_string(n);
	label->setString(labelstr);

	Main* main = (Main*)cocos2d::Director::getInstance()->getRunningScene()->getChildByName("main");
	main->pr_col = n;
	main->update_price(popUp);
}
void Main::update_price(cocos2d::Layer* pop) {
	int d = this->pr_att*ATT_DIAM + this->pr_res*RES_DIAM + this->pr_col*COL_DIAM;
	int e = this->pr_att*ATT_ENER + this->pr_res*RES_ENER + this->pr_col*COL_ENER;
	int p = this->pr_att*ATT_POWD + this->pr_res*RES_POWD + this->pr_col*COL_POWD;

	Label* labelp = (Label*)pop->getChildByName("price");
	Button* btn = (Button*)pop->getChildByName("buybtn");
	if (this->playerEmpire->diam < d || this->playerEmpire->ener < e || this->playerEmpire->powd < p) {
		labelp->setColor(Color3B(255,0,0));
		if (btn->isEnabled()) {
			btn->setEnabled(false);
		}
	} else {
		labelp->setColor(Color3B(255, 255, 255));
		if (!btn->isEnabled()) {
			btn->setEnabled(true);
		}
	}
	
	std::string labelstr = "Overall price: " + std::to_string(d) + " " + std::to_string(e) + " " + std::to_string(p);
	labelp->setString(labelstr);
}