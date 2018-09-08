#pragma once
#include "Entity.h"
#include "Animation.h"

class Item :
	public Entity
{
public:
	Item(ItemType type);
	~Item();

	ItemType type;
	Animation *animation;

	float moveRotation;
	float gravity;
	float speed;
	float effectTimer;

	void update(float dt);
};

