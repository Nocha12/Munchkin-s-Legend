#include "DXUT.h"
#include "Emitter.h"

Emitter::Emitter(path spritePath, bool isBlending)
	:spritePath(spritePath), isBlending(isBlending), started(false), currentCount(0), time(0), rotationSpeed(0), rotationOffset(0)
{
	this->color(D3DXCOLOR(1,1,1,1), D3DXCOLOR(1,1,1,1))
		->speed(0, 0)
		->scale(1, 1)
		->lifeTime(0, 0)
		->rotation(-D3DX_PI, D3DX_PI);
}


Emitter::~Emitter(void)
{
}


void Emitter::start(float frequency, int quantity, int emitCount)
{
	this->frequency = frequency;
	this->quantity = quantity;
	this->emitCount = emitCount;
	started = true;
}


void Emitter::createParticle()
{
	float rotation = random( rotationMin, rotationMax )+rotationOffset;
	Vec2 speed = Vec2( random(speedMin,speedMax)*cos(rotation), random(speedMin,speedMax)*sin(rotation) );
	float scale = random(scaleMin, scaleMax);
	float lifeTime = random(lifeTimeMin, lifeTimeMax);
	Sprite* s = new Sprite( spritePath );
	s->scale = Vec2( scale, scale );
	s->blending = isBlending;

	Particle p = {s, speed, 0, lifeTime, rotationSpeed};
	particles.push_back(p);
	addChild(s);
}


bool Emitter::isEnd()
{
	return (currentCount >= emitCount) && particles.empty();
}


Emitter* Emitter::color(D3DXCOLOR from, D3DXCOLOR to)
{
	colorFrom = from;
	colorTo = to;
	return this;
}

Emitter* Emitter::speed(float min, float max)
{
	speedMin = min;
	speedMax = max;
	return this;
}

Emitter* Emitter::scale(float min, float max)
{
	scaleMin = min;
	scaleMax = max;
	return this;
}

Emitter* Emitter::lifeTime(float min, float max)
{
	lifeTimeMin = min;
	lifeTimeMax = max;
	return this;
}

Emitter* Emitter::rotation(float min, float max)
{
	rotationMin = min;
	rotationMax = max;
	return this;
}

void Emitter::update(float dt)
{
	Entity::update(dt);
	if(!started || isEnd()) return;

	time+=dt;

	for_iter(iter, particles) {

		iter->lifeTime += dt;
		iter->sprite->pos += iter->speed*dt;
		iter->sprite->rotation += rotationSpeed*dt;

		iter->sprite->color = colorFrom+(colorTo-colorFrom)*(iter->lifeTime/iter->lifeTimeOrigin);

		if(iter->lifeTime > iter->lifeTimeOrigin) {
			removeChild(iter->sprite);
			iter = particles.erase(iter);
			if(iter == particles.end())
				break;
		}
	}

	if( time >= frequency )
	{
		time = 0;
		currentCount++;
		if(currentCount >= emitCount)
			return;
		for( int i = 0; i < quantity; i++ )
			createParticle();
	}
}
