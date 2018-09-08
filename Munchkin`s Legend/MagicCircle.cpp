#include "DXUT.h"
#include "MagicCircle.h"
#include "World.h"
#include "GameScene.h"

MagicCircle::MagicCircle() : timer(0), isBinded(false)
{
	addChild(magicCircle = new Sprite("image/effect/mob/magicCircle.png"));
	rect = magicCircle->rect;
	rotationCenter = Vec2(center().x, center().y - 50);
	scaleCenter = Vec2(center().x, center().y - 50);
	scale = Vec2(0, 0);

	scaleSizeTimer1.reset(0.01f, 100);
	scaleSizeTimer1.onTick = [=]()
	{
		scale = lerp(scale, Vec2(1, 1), (float)scaleSizeTimer1.count.first / 100);
	};
}

MagicCircle::~MagicCircle()
{
}

void MagicCircle::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;
	Entity::update(dt);

	if (timer < 1.5f)
		timer += dt;

	if (timer > 1.5f && timer < 3 && collision(scene->player->ColPivotWithPos(), pos + scaleCenter, 200) && !isBinded)
	{
		isBinded = true;
		scene->player->isBinding = true;
	}

	if (isBinded)
		timer += dt;

	if (timer > 3 && isBinded)
	{
		isBinded = false;
		scene->player->isBinding = false;
		scaleSizeTimer2.reset(0.01f, 50);

		scaleSizeTimer2.onTick = [=]()
		{
			scale = lerp(scale, Vec2(0, 0), (float)scaleSizeTimer2.count.first / 50);
		};
	}

	if (scale.x <= 0.03 && timer >= 3)
	{
		scene->magicCircleList.remove(this);
		removeChild(this);
	}
}
