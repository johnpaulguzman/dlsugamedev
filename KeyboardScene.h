#pragma once

#include "cocos2d.h"
#include <map>
#include <vector>
#include "SimpleAudioEngine.h"

class Snekbody {
public:
	int dir;
	float x;
	float y;
	cocos2d::Sprite* sprite;
	Snekbody(int pdir, float px, float py, int pnum) { 
		dir = pdir; 
		x = px; 
		y = py; 
		if(pnum==1)
			sprite = cocos2d::Sprite::create("body.png");
		else if(pnum==2)
			sprite = cocos2d::Sprite::create("body2.png");
		sprite->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
		sprite->setPosition(px, py);
		
	}
};

class KeyboardScene : public cocos2d::Layer
{

public:
	cocos2d::Sprite* sprite = cocos2d::Sprite::create("head.png");
	cocos2d::Sprite* sprite2 = cocos2d::Sprite::create("head2.png");
	cocos2d::Sprite* food = cocos2d::Sprite::create("food.png");
	cocos2d::Sprite* bg = cocos2d::Sprite::create("walls.png");
	CocosDenshion::SimpleAudioEngine* audio = CocosDenshion::SimpleAudioEngine::getInstance();
	cocos2d::Label* showscore = cocos2d::Label::createWithSystemFont("Player 1: 0", "Arial", 20);
	cocos2d::Label* showscore2 = cocos2d::Label::createWithSystemFont("Player 2: 0", "Arial", 20);
	int dir = 2;
	int cooldown =- 1;
	int cdm = -1;
	int dir2 = 1;
	int cooldown2 = -1;
	int cdm2 = -1;
	int sw = 1200;
	int sh = 600;
	int ss = 30;
	int score = 0;
	int score2 = 0;
	bool live = true;
	bool live2 = true;
	std::vector<Snekbody> sbody;
	std::vector<Snekbody> sbody2;
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(KeyboardScene);
	cocos2d::Vec2 movedir(int, cocos2d::Vec2);
	bool collides(cocos2d::Vec2, cocos2d::Vec2);
	void updateScore(int, std::string);
	void die(int);
	void update(float) override;

private:
    static std::map<cocos2d::EventKeyboard::KeyCode,
        std::chrono::high_resolution_clock::time_point> keys;
    cocos2d::Label* label;
};
