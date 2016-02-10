#include "KeyboardScene.h"
#include <vector>
USING_NS_CC;

void KeyboardScene::updateScore(int pnum, std::string type) {
	auto dscore = 0;
	if (type == "Die") {
		dscore = -1;
		if (pnum == 1)
			live = false;
		else if (pnum == 2)
			live2 = false;
	}
	else if (type == "Eat")
		dscore = 2;
	if (pnum == 1) {
		score += dscore;
		showscore->setString("Player 1: "+ std::to_string(score));
	}
	else if (pnum == 2) {
		score2 += dscore;
		showscore2->setString("Player 2: " + std::to_string(score2));
	}
}

void KeyboardScene::die(int pnum) {
	if (pnum == 1) {
		for (int l = 0; l < sbody.size(); l++)
			sbody.at(l).sprite->setPosition(1230, 1230);
		sbody.clear();
		sprite->setPosition(ss * 2, ss * 2);
		dir = 2;
		updateScore(1, "Die");
	}
	else if (pnum == 2) {
		for (int l = 0; l < sbody2.size(); l++) 
			sbody2.at(l).sprite->setPosition(1230, 1230);
		sbody2.clear();
		sprite2->setPosition(sw - ss * 2, sh - ss * 2);
		dir2 = 1;
		updateScore(2, "Die");
	}
	audio->playEffect("die.mp3");
}

bool KeyboardScene::collides(cocos2d::Vec2 pos, cocos2d::Vec2 pos2) {
	return (std::abs(pos.x - pos2.x) < ss && std::abs(pos.y - pos2.y) < ss);
}

cocos2d::Vec2 KeyboardScene::movedir(int dir, cocos2d::Vec2 position) {
	switch (dir) {
	case 1:
		position.x -= ss;
		break;
	case 2:
		position.x += ss;
		break;
	case 3:
		position.y += ss;
		break;
	case 4:
		position.y -= ss;
		break;
	}
	return position;
}

Scene* KeyboardScene::createScene(){
	auto scene = Scene::create();
	auto layer = KeyboardScene::create();
	scene->addChild(layer);
	return scene;
}

bool KeyboardScene::init(){
	if (!Layer::init())
		return false;
	audio->preloadBackgroundMusic("fight.mp3");
	audio->preloadEffect("die.mp3");
	audio->preloadEffect("eat.mp3");
	audio->playBackgroundMusic("fight.mp3");
	bg->setPosition(sw/2, sh/2);
	bg->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(bg);
	sprite->setAnchorPoint(Vec2(0.5, 0.5));
	sprite->setPosition(ss*2, ss*2);
	this->addChild(sprite, 0);
	sprite2->setAnchorPoint(Vec2(0.5, 0.5));
	sprite2->setPosition(sw-ss * 2, sh-ss * 2);
	this->addChild(sprite2, 0);
	food->setPosition(sw/2, sh/2);
	food->setAnchorPoint(Vec2(0.5, 0.5));
	this->addChild(food, 0);
	showscore->setPosition(125, 20);
	this->addChild(showscore);
	showscore2->setPosition(sw-125, sh-20);
	this->addChild(showscore2);
	auto eventListener = EventListenerKeyboard::create();
	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		//control 1
		switch (keyCode) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			if(dir!=2 && cdm<0)
				dir = 1;
			cdm = 0;
			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			if (dir != 1 && cdm<0)
				dir = 2;
			cdm = 0;
			break;
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			if (dir != 4 && cdm<0)
				dir = 3;
			cdm = 0;
			break;
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
			if (dir != 3 && cdm<0)
				dir = 4;
			cdm = 0;
			break;
		//control 2
		case EventKeyboard::KeyCode::KEY_A:
			if (dir2 != 2 && cdm2<0)
				dir2 = 1;
			cdm2 = 0;
			break;
		case EventKeyboard::KeyCode::KEY_D:
			if (dir2 != 1 && cdm2<0)
				dir2 = 2;
			cdm = 0;
			break;
		case EventKeyboard::KeyCode::KEY_W:
			if (dir2 != 4 && cdm2<0)
				dir2 = 3;
			cdm2 = 0;
			break;
		case EventKeyboard::KeyCode::KEY_S:
			if (dir2 != 3 && cdm2<0)
				dir2 = 4;
			cdm2 = 0;
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, sprite);
	this->scheduleUpdate();
	return true;
}

void KeyboardScene::update(float delta) {
	live = true;
	live2 = true;
	auto foodpos = food->getPosition();
	auto position = sprite->getPosition();
	auto position2 = sprite2->getPosition();
	auto dolook = false;
	//food eat1
	if (collides(position, foodpos) && cooldown<0) {
		cooldown = 1;
		sbody.insert(sbody.begin(), Snekbody(dir, position.x, position.y, 1));
		this->addChild(sbody.at(0).sprite, 0);
		sprite->setPosition(movedir(dir, position));
		updateScore(1, "Eat");
		audio->playEffect("eat.mp3");
		dolook = true;
	}
	//food eat2
	if (collides(position2, foodpos) && cooldown2<0) {
		cooldown2 = 1;
		sbody2.insert(sbody2.begin(), Snekbody(dir2, position2.x, position2.y, 2));
		this->addChild(sbody2.at(0).sprite, 0);
		sprite2->setPosition(movedir(dir2, position2));
		updateScore(2, "Eat");
		audio->playEffect("eat.mp3");
		dolook = true;
	}
	//spawn food
	auto rpos = position;
	if (dolook) {
		auto lookpos = true;
		while (lookpos) {
			lookpos = false;
			rpos.x = std::rand() % (sw - ss * 4) + 2 * ss;
			rpos.y = std::rand() % (sh - ss * 4) + 2 * ss;
			if (collides(position, rpos) || collides(position2, rpos))
				lookpos = true;
			for (int l = 0; l < sbody.size(); l++)
				if (collides(rpos, sbody.at(l).sprite->getPosition()))
					lookpos = true;
			for (int l = 0; l < sbody2.size(); l++) 
				if (collides(rpos, sbody2.at(l).sprite->getPosition()))
					lookpos = true;
		}
		food->setPosition(rpos);
		return;
	}
	//head on collision
	auto stale = collides(position, position2);
	auto headinu = false;
	auto headinme = false;
	for (int s1 = 0; s1 < sbody.size(); s1++) 
		if (collides(position2, sbody.at(s1).sprite->getPosition()))
			headinme = true;
	for (int s1 = 0; s1 < sbody2.size(); s1++) 
		if (collides(position, sbody2.at(s1).sprite->getPosition()))
			headinme = true;
	if ((stale) || (headinu && headinme)) {
		if (sbody.size() < sbody2.size())
			die(1);
		else if (sbody.size() > sbody2.size())
			die(2);
	}
	else {
		//die1
		auto collision = false;
		collision = position.x<ss || position.x>sw - ss || position.y<ss || position.y>sh - ss;
		for (int l = 0; l < sbody.size(); l++)
			if (collides(position, sbody.at(l).sprite->getPosition()))
				collision = true;
		for (int l = 0; l < sbody2.size(); l++)
			if (collides(position, sbody2.at(l).sprite->getPosition()))
				collision = true;
		if (collision)
			die(1);
		//die 2
		auto collision2 = false;
		collision2 = position2.x<ss || position2.x>sw - ss || position2.y<ss || position2.y>sh - ss;
		for (int l = 0; l < sbody2.size(); l++)
			if (collides(position2, sbody2.at(l).sprite->getPosition()))
				collision2 = true;
		for (int l2 = 0; l2 < sbody.size(); l2++)
			if (collides(position2, sbody.at(l2).sprite->getPosition()))
				collision2 = true;
		if (collision2)
			die(2);
	}
	//move headbody 12
	if (live) {
		sprite->setPosition(movedir(dir, position));
		for (int i = 0; i < sbody.size(); i++)
			sbody.at(i).sprite->setPosition(movedir(sbody.at(i).dir, sbody.at(i).sprite->getPosition()));
		for (int i = sbody.size() - 1; i > -1; i--) {
			if (i == 0)
				sbody.at(0).dir = dir;
			else
				sbody.at(i).dir = sbody.at(i - 1).dir;
		}
	}
	if (live2) {
		sprite2->setPosition(movedir(dir2, position2));
		for (int i = 0; i < sbody2.size(); i++)
			sbody2.at(i).sprite->setPosition(movedir(sbody2.at(i).dir, sbody2.at(i).sprite->getPosition()));
		for (int i = sbody2.size() - 1; i > -1; i--) {
			if (i == 0)
				sbody2.at(0).dir = dir2;
			else
				sbody2.at(i).dir = sbody2.at(i - 1).dir;
		}
	}
	//update cooldown timers
	cooldown--;
	cdm--;
	cooldown2--;
	cdm2--;
}
