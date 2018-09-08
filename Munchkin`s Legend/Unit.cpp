#include "DXUT.h"
#include "Unit.h"
#include "World.h"
#include "MultiDirAnimation.h"
#include "GameScene.h"
#include "Item.h"

Unit::Unit() : moveRotation(0), backSpeed(0), attackFrame1(0), attackFrame2(0), isAttacked1(false), isAttacked2(false), attackRange(0), scanRange(0), power(0), isMap(false), isSetMinimapImg(false)
{
}

Unit::~Unit()
{
}

void Unit::update(float dt)
{
	Object::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;
	
	if (isMap) return;

	minimapImg->setCenter(center() / 15 + Vec2(1019, 21));

	if (ColPivotWithPos().y < -(888 / 1775.0f) * ColPivotWithPos().x + 950)
		pos += Vec2(cosf(D3DXToRadian(45)), sinf(D3DXToRadian(45))) * speed * dt;
	if (ColPivotWithPos().y < (888 / 1775.0f) * ColPivotWithPos().x - 830)
		pos += Vec2(cosf(D3DXToRadian(135)), sinf(D3DXToRadian(135))) * speed * dt;
	if (ColPivotWithPos().y > -(888 / 1775.0f) * ColPivotWithPos().x + 2604)
		pos += Vec2(cosf(D3DXToRadian(-135)), sinf(D3DXToRadian(-135))) * speed * dt;
	if (ColPivotWithPos().y > (888 / 1775.0f) * ColPivotWithPos().x + 810)
		pos += Vec2(cosf(D3DXToRadian(-45)), sinf(D3DXToRadian(-45))) * speed * dt;

	if (backSpeed > 0)
	{
		pos -= Vec2(cosf(moveRotation), sinf(moveRotation)) * backSpeed * dt;
		backSpeed = backSpeed - (backSpeed + 30) * 2 * dt < 0 ? 0 : backSpeed - (backSpeed + 30) * 2 * dt;
	}

	if (this == scene->player) return;

	if (!isDead && hp <= 0)
	{
		isDead = true;
		hp = 0;
		ChangeState(DEAD, true);
		((MultiDirAnimation*)animationMap[DEAD])->FaidOut();
		removeChild(minimapImg);

		for (int i = 0; i < 5; i++)
		{
			Item *item = nullptr;
			
			if (random(0, 20) == 0)
			{
				if (random(0, 1) == 0)
					item = new Item(HpPotion);
				else 
					item = new Item(MpPotion);
			}
			else 
				item = new Item(GoldItem);
			scene->addChild(item);
			scene->itemList.push_back(item);
			item->setCenter(center());
		}
	}

	if (!removing && isDead && ((MultiDirAnimation*)animationMap[DEAD])->animation[0]->color.a <= 0.05f)
	{
		if (type == SLIME)
			--scene->enemyManager->slimeCount;
		if (type == SKULLKNIGHT)
			--scene->enemyManager->skullKnightCount;
		if (type == BRONZEKNIGHT)
			--scene->enemyManager->bronzeKnightCount;
		if (type == SILVERKNIGHT)
			--scene->enemyManager->silverKnightCount;
		if (type == GOLDKNIGHT)
			--scene->enemyManager->goldKnightCount;
		if (type == BOSS)
			--scene->enemyManager->bossCount;

		scene->unitList.remove(this);
		removeChild(this);
	}
}

void Unit::ChangeState(StateAnimationType type, bool resetAni)
{
	for_iter(ani, animationMap)
	{
		if ((*ani).first == type) continue;

		(*ani).second->visible = false;
		((MultiDirAnimation*)(*ani).second)->Reset();
		((MultiDirAnimation*)(*ani).second)->Stop();
	}

	if (resetAni)
		((MultiDirAnimation*)animationMap[type])->Reset();

	animationMap[type]->visible = true;

	((MultiDirAnimation*)animationMap[type])->Play();
}

void Unit::GetDamage(int power, bool isPushBack, float shild)
{
	GameScene *scene = (GameScene*)world.currentScene;

	if (type != PLAYER)
	{
		Sprite *img = new Sprite("image/effect/player/skill1.png");
		scene->addChild(img);
		scene->effectList.push_back(img);
		img->setCenter(center());
		img->rotationCenter = img->rect.center();
		img->scaleCenter = img->rect.center();
		img->scale = Vec2(0.2f, 0.2f);
		img->rotation = D3DXToRadian(random(-50, 50));
	}

	if (isPushBack)
		backSpeed = 200;

	for_iter(ani, animationMap)
		((MultiDirAnimation*)ani->second)->GetRed();

	hp -= power - shild < 0 ? 0 : power - shild;
}

TreeBottom::TreeBottom()
{
	isMap = true;
	Sprite *sprite = new Sprite("image/map/1/ob1.png");
	addChild(sprite);
	coliderRange = 25;
	coliderPivot = Vec2(65, 115);
}

TreeBottom::~TreeBottom() { }

Rock::Rock()
{
	isMap = true;
	Sprite *sprite = new Sprite("image/map/1/ob2.png");
	addChild(sprite);
	coliderRange = 34;
	coliderPivot = Vec2(56, 64);
}

Rock::~Rock() { }

Pillar::Pillar()
{
	isMap = true;
	sprite = new Sprite("image/map/3/ob1.png");
	rect = sprite->rect;
	addChild(sprite);
	coliderRange = 25;
	coliderPivot = Vec2(79, 354);
}

Pillar::~Pillar() { }

void Pillar::update(float dt)
{
	Unit::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	if (scene->player->ColPivotWithPos() < ColPivotWithPos() && rectWithPos().intersects(scene->player->ColPivotWithPos()))
		sprite->color.a = sprite->color.a - dt * 2 > 0.5f ? sprite->color.a - dt * 2 : 0.5f;
	else
		sprite->color.a = sprite->color.a + dt * 2 < 1 ? sprite->color.a + dt * 2 : 1; 
}

House::House(int type)
{
	isMap = true;
	if (type == 0)
	{
		Sprite *sprite = new Sprite("image/map/2/ob1.png");
		addChild(sprite);
		coliderRange = 220;
		coliderPivot = Vec2(355, 258);
	}
	else
	{
		Sprite *sprite = new Sprite("image/map/2/ob2.png");
		addChild(sprite);
		coliderRange = 220;
		coliderPivot = Vec2(326, 213);
	}
}

House::~House() { }

Fountain::Fountain()
{
	isMap = true;
	Sprite *sprite = new Sprite("image/map/2/ob3.png");
	addChild(sprite);
	coliderRange = 140;
	coliderPivot = Vec2(191, 212);
}

Fountain::~Fountain() { }

Tree::Tree(int type)
{
	isMap = true;
	if (type == 0)
	{
		sprite = new Sprite("image/map/2/ob4.png");
		rect = sprite->rect;
		addChild(sprite);
		coliderRange = 0;
		coliderPivot = Vec2(155, 232);
	}
	else
	{
		sprite = new Sprite("image/map/2/ob5.png");
		rect = sprite->rect;
		addChild(sprite);
		coliderRange = 0;
		coliderPivot = Vec2(188, 305);
	}
}

Tree::~Tree() { }

void Tree::update(float dt)
{
	Unit::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	if (scene->player->ColPivotWithPos() < ColPivotWithPos() && rectWithPos().intersects(scene->player->ColPivotWithPos()))
		sprite->color.a = sprite->color.a - dt * 2 > 0.5f ? sprite->color.a - dt * 2 : 0.5f;
	else
		sprite->color.a = sprite->color.a + dt * 2 < 1 ? sprite->color.a + dt * 2 : 1;
}