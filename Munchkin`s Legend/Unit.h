#pragma once
#include "Object.h"
#include "Sprite.h"

class Unit : public Object
{
public:
	Unit();
	~Unit();

	void update(float dt);

	float moveRotation;

	float hp;
	float maxHp;

	float backSpeed;
	float attackFrame1;
	float attackFrame2;
	float power;

	bool isSetMinimapImg;
	bool isAttacked1;
	bool isAttacked2;
	bool isMap;
	int attackRange, scanRange;

	void GetDamage(int power, bool isPushBack = true, float shild = 0);

	UnitType type;
	Sprite *minimapImg;

	map<StateAnimationType, Entity*> animationMap;
	void ChangeState(StateAnimationType type, bool resetAni = false);
};

class TreeBottom : public Unit
{
public:
	TreeBottom();
	~TreeBottom();
};

class Rock : public Unit
{
public:
	Rock();
	~Rock();
};

class Pillar : public Unit
{
public:
	Pillar();
	~Pillar();

	Sprite *sprite;

	void update(float dt);
}; 

class House : public Unit
{
public:
	House(int type);
	~House();
};

class Fountain : public Unit
{
public:
	Fountain();
	~Fountain();
};

class Tree : public Unit
{
public:
	Tree(int type);
	~Tree();

	Sprite *sprite;

	void update(float dt);
};