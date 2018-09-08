#pragma once
#include "Entity.h"
#include "Sprite.h"
#include "Timer.h"

class MagicCircle : public Entity
{
public:
	MagicCircle();
	~MagicCircle();

	Timer scaleSizeTimer1;
	Timer scaleSizeTimer2;
	Sprite *magicCircle;

	bool isBinded;
	float timer;

	void update(float dt);
};

