#include "DXUT.h"
#include "Object.h"
#include "GameScene.h"
#include "World.h"

Object::Object() : speed(0), isDead(false), coliderPivot(0, 0), coliderRange(0), isStand(false)
{
}


Object::~Object()
{
}

void Object::update(float dt)
{
	Entity::update(dt);

	if (isDead) return;

	GameScene *scene = (GameScene*)world.currentScene;

	for_iter(obj, scene->unitList)
	{
		if (*obj == this) continue;

		if (collision(ColPivotWithPos(), (*obj)->ColPivotWithPos(), coliderRange + (*obj)->coliderRange) && !(*obj)->isDead && !isStand)
		{
			float t = angle(ColPivotWithPos(), (*obj)->ColPivotWithPos());

			pos -= Vec2(cosf(t), sinf(t)) * speed * dt;
		}
	}
}

Vec2 Object::ColPivotWithPos()
{
	return pos + coliderPivot;
}