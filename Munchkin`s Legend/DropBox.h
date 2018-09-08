#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Timer.h"

class DropBox : public Entity
{
public:
	DropBox(int stage);
	~DropBox();

	int stage;
	bool isFaidIn;
	bool isDropItem;
	Animation *ani;
	Timer makeItemTimer;

	void update(float dt);
};

