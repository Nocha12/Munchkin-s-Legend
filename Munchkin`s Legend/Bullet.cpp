#include "DXUT.h"
#include "Bullet.h"
#include "Sprite.h"
#include "World.h"
#include "GameScene.h"

Bullet::Bullet(BulletType type, float angle, float speed, float speedRate, float damage) : type(type), angle(angle), speed(speed), speedRate(speedRate), damage(damage), removed(false)
{
	Sprite *t = nullptr;

	if (type == SlimeBullet)
		addChild(t = new Sprite("image/effect/mob/0.png"));
	if (type == BossBullet)
		addChild(t = new Sprite("image/effect/mob/bossCircle.png"));

	rect = t->rect;
	range = rect.width() / 2;
	rotationCenter = center();
}

Bullet::~Bullet()
{
}

void Bullet::update(float dt)
{
	if (removed)
	{
		removeChild(this);
		((GameScene*)world.currentScene)->bulletList.remove(this);
	}

	Entity::update(dt);

	pos += Vec2(cos(angle), sin(angle)) * speed * dt;
	speed += speedRate;
}