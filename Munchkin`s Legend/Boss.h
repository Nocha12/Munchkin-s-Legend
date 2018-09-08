#pragma once
#include "Monster.h"
#include "Animation.h"
#include "Timer.h"

class SitBoss : public Unit
{
public: 
	SitBoss();
	~SitBoss();

	void update(float dt);

	Animation *sitAni; 
	Animation *attackAni;

	bool isAttacked;
	Vec2 pivot;
};

class Boss : public Monster
{
public:
	Boss();
	~Boss();

	void update(float dt);
	void Attack();

	int backDashState;
	float backDashTimer;
	float delay;

	Timer afterImageTimer;
};

