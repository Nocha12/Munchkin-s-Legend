#pragma once
#include "Unit.h"
#include "Sprite.h"

class Monster : public Unit
{
public:
	Monster();
	~Monster();
	
	bool isFollowing;
	bool canAttack;

	Sprite *hpBarBack;
	Sprite *hpBar;

	void update(float dt);
	virtual void Attack() = 0;
};

class Slime : public Monster
{
public:
	Slime();
	~Slime();

	float changeDirTimer;

	void update(float dt);
};

class SmallSlime : public Slime 
{
public:
	SmallSlime();
	~SmallSlime();

	void Attack();
};

class BigSlime : public Slime
{
public:
	BigSlime();
	~BigSlime();

	void update(float dt);
	void Attack();
};

class Knight : public Monster
{
public:
	Knight(KnightType type);
	~Knight();

	void Attack();
};
