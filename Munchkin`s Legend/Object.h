#pragma once
#include "Entity.h"

class Object : public Entity
{
public:
	Object();
	~Object();

	void update(float dt);

	int speed;
	bool isStand;
	bool isDead;

	Vec2 coliderPivot;
	int coliderRange;

	Vec2 ColPivotWithPos();
};

