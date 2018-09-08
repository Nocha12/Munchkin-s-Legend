#include "DXUT.h"
#include "Item.h"
#include "World.h"
#include "GameScene.h"

Item::Item(ItemType type) : type(type), gravity(-150), speed(80), effectTimer(0)
{
	animation = nullptr;

	moveRotation = D3DXToRadian(random(0, 360));

	if (type == GoldItem)
		addChild(animation = new Animation("image/item/gold", 10, 10, true, true));
	else if (type == HpPotion)
		addChild(animation = new Animation("image/item/hp", 10, 10, true, true));
	else if (type == MpPotion)
		addChild(animation = new Animation("image/item/mp", 10, 10, true, true));
	else if (type == Necklace)
		addChild(animation = new Animation("image/item/necklace", 10, 10, true, true));
	else if (type == Ring)
		addChild(animation = new Animation("image/item/ring", 10, 10, true, true));

	rect = animation->rect;
}

Item::~Item()
{
}

void Item::update(float dt)
{
	Entity::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	effectTimer += dt;

	if (effectTimer > 0.2f && (type == Necklace || type == Ring))
	{
		effectTimer = 0;
		Sprite *img = new Sprite("image/UI/itemEffect.png");
		scene->addChild(img);
		scene->hpMpEffectList.push_back(img);
		img->setCenter(center() + Vec2(random(-25, 25), random(20, 50)));
	}

	if (gravity < 160)
	{
		pos.y += (gravity += 9.8f) * dt;
		pos += Vec2(cos(moveRotation), sin(moveRotation)) * dt * speed;
	}
	else if(collision(scene->player->center(), center(), 150))
	{
		moveRotation = angle(center(), scene->player->center());
		pos += Vec2(cos(moveRotation), sin(moveRotation)) * dt * (speed += dt * 500);
	}
}
