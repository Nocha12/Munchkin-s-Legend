#pragma once
#include "Entity.h"

class Bullet : public Entity
{
public:
	Bullet(BulletType type, float angle, float speed, float speedRate, float damage = 10);
	~Bullet();

	float angle;
	float speed;
	float speedRate;
	float damage;
	float range;
	bool removed;

	BulletType type;

	void update(float dt);
};

