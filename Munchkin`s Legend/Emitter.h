#pragma once
#include <limits>
#include "define.h"
#include "Entity.h"
#include "Sprite.h"

typedef struct {
	Sprite* sprite;
	D3DXVECTOR2 speed;
	float lifeTime;
	float lifeTimeOrigin;
	float rotationSpeed;
} Particle;

class Emitter :
	public Entity
{
public:
	Emitter(path spritePath, bool isBlending);
	~Emitter(void);
	void update(float dt);

	void start(float frequency, int quantity, int emitCount = INT_MAX);
	void createParticle();
	bool isEnd();

	Emitter* color(D3DXCOLOR from, D3DXCOLOR to);
	Emitter* speed(float min, float max);
	Emitter* scale(float min, float max);
	Emitter* lifeTime(float min, float max);
	Emitter* rotation(float min, float max);

	D3DXCOLOR colorFrom, colorTo;
	float scaleMin, scaleMax;
	float speedMin, speedMax;
	float lifeTimeMin, lifeTimeMax;
	float rotationMin, rotationMax;
	float rotationOffset;
	float rotationSpeed;
	bool isBlending;

	list<Particle> particles;
	path spritePath;

	float time;
	float frequency;
	int quantity;
	int emitCount;
	int currentCount;
	bool started;
};

