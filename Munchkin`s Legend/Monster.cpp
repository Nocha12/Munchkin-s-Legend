#include "DXUT.h"
#include "Monster.h"
#include "MultiDirAnimation.h"
#include "GameScene.h"
#include "World.h"
#include "Bullet.h"

Monster::Monster() : isFollowing(false), canAttack(true)
{
}

Monster::~Monster()
{
}

void Monster::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	if (!isSetMinimapImg)
	{
		if (type == BOSS)
		{
			scene->ui->addChild(minimapImg = new Sprite("image/UI/minimap/boss.png"));
			addChild(hpBarBack = new Sprite("image/UI/mobHpBar/boss/bar2.png"));
			addChild(hpBar = new Sprite("image/UI/mobHpBar/boss/bar1.png"));
			hpBarBack->pos = Vec2(353, 269);
			hpBar->pos = Vec2(356, 271);
		}
		else if (type != PLAYER)
		{
			scene->ui->addChild(minimapImg = new Sprite("image/UI/minimap/enemy.png"));
			addChild(hpBarBack = new Sprite("image/UI/mobHpBar/mob/bar2.png"));
			addChild(hpBar = new Sprite("image/UI/mobHpBar/mob/bar1.png"));

			if (type == SLIME)
			{
				hpBarBack->pos = Vec2(154, 157);
				hpBar->pos = Vec2(156, 159);
			}
			else if (type == BIGSLIME)
			{
				hpBarBack->pos = Vec2(150, 96);
				hpBar->pos = Vec2(152, 98);
			}
			else if (type == SKULLKNIGHT)
			{
				hpBarBack->pos = Vec2(153, 90);
				hpBar->pos = Vec2(155, 92);
			} else {
				hpBarBack->pos = Vec2(223, 143);
				hpBar->pos = Vec2(225, 145);
			}
		}
		isSetMinimapImg = true;
	}

	hpBar->visibleRect.right = hpBar->rect.right * hp / maxHp;

	Unit::update(dt);

	if (isDead) return;

	float moveSpeed = speed;

	if (collision(ColPivotWithPos(), scene->player->ColPivotWithPos(), scene->player->coliderRange + attackRange) && backSpeed == 0 && canAttack)
	{
		Attack();
		moveSpeed = 0;
		moveRotation = angle(ColPivotWithPos(), scene->player->ColPivotWithPos());
	}
	else if (isFollowing)
	{
		ChangeState(WALK);
		moveRotation = angle(ColPivotWithPos(), scene->player->ColPivotWithPos());
		if (backSpeed <= 50)
			pos += Vec2(cos(moveRotation), sin(moveRotation)) * moveSpeed * dt;
	}
}

Slime::Slime() : changeDirTimer(0) { }
Slime::~Slime() { }

void Slime::update(float dt)
{
	Monster::update(dt);

	GameScene *scene = (GameScene*)world.currentScene;

	if (collision(ColPivotWithPos(), scene->player->ColPivotWithPos(), scene->player->coliderRange + scanRange))
		isFollowing = true;
	else
	{
		isFollowing = false;

		changeDirTimer += dt;

		if (changeDirTimer > 3)
		{
			changeDirTimer = 0;
			moveRotation = random(D3DXToRadian(0), D3DXToRadian(360));
		}

		pos += Vec2(cos(moveRotation), sin(moveRotation)) * (speed - 50) * dt;
	}
}

SmallSlime::SmallSlime()
{
	type = SLIME;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/smallSlime/move", 10, 10, true));
	rect = animationMap[WALK]->rect;
	addChild(animationMap[ATTACK1] = new MultiDirAnimation(this, "image/monster/smallSlime/attack", 8, 8, true));
	animationMap[ATTACK1]->visible = false;
	addChild(animationMap[DEAD] = new MultiDirAnimation(this, "image/monster/smallSlime/die", 9, 10, false));
	animationMap[DEAD]->visible = false;
	speed = 140;
	power = 1;
	coliderPivot = Vec2(189, 219);
	coliderRange = 20;
	attackRange = 60;
	scanRange = 350;

	maxHp = hp = 5;
}

SmallSlime::~SmallSlime()
{
}

void SmallSlime::Attack()
{
	ChangeState(ATTACK1);
	GameScene *scene = (GameScene*)world.currentScene;
	MultiDirAnimation *attackAni1 = (MultiDirAnimation*)animationMap[ATTACK1];

	if ((int)attackAni1->currentFrame == 5 && !isAttacked1 && !scene->player->isAvoiding)
	{
		scene->player->GetDamage(power, false);
		isAttacked1 = true;
	}
	else if (!((int)attackAni1->currentFrame == 5))
		isAttacked1 = false;
}

BigSlime::BigSlime()
{
	type = BIGSLIME;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/bigSlime/move", 10, 10, true));
	rect = animationMap[WALK]->rect;
	addChild(animationMap[ATTACK1] = new MultiDirAnimation(this, "image/monster/bigSlime/attack", 8, 8, true));
	animationMap[ATTACK1]->visible = false;
	addChild(animationMap[DEAD] = new MultiDirAnimation(this, "image/monster/bigSlime/die", 9, 10, false));
	animationMap[DEAD]->visible = false;
	speed = 150;
	power = 3;
	coliderPivot = Vec2(189, 213);
	coliderRange = 48;
	attackRange = 250;
	scanRange = 350;

	maxHp = hp = 10;
}

BigSlime::~BigSlime()
{
}

void BigSlime::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;
	Slime::update(dt);

	if (hp <= 0 && !removing)
	{
		--scene->enemyManager->bigSlimeCount;
		removeChild(this);
		scene->unitList.remove(this);

		for (int i = 0; i < 4; i++)
		{
			SmallSlime *slime = new SmallSlime();
			scene->addChild(slime);
			scene->unitList.push_back(slime);
			slime->setCenter(center());
			++scene->enemyManager->slimeCount;
			slime->backSpeed = slime->speed;
		}
	}
}

void BigSlime::Attack()
{
	ChangeState(ATTACK1);
	GameScene *scene = (GameScene*)world.currentScene;
	MultiDirAnimation *attackAni1 = (MultiDirAnimation*)animationMap[ATTACK1];

	if (collision(ColPivotWithPos(), scene->player->ColPivotWithPos(), scene->player->coliderRange + 80))
	{
		if ((int)attackAni1->currentFrame == 6 && !isAttacked1 && !scene->player->isAvoiding)
		{
			isAttacked1 = true;
			scene->player->GetDamage(power, false);
		}
		else if (!((int)attackAni1->currentFrame == 6))
			isAttacked1 = false;
	}
	else
	{
		if ((int)attackAni1->currentFrame == 6 && !isAttacked2)
		{
			isAttacked2 = true;

			Bullet *bullet = new Bullet(SlimeBullet, angle(center(), scene->player->center()), 100, 0, power);
			scene->addChild(bullet);
			scene->bulletList.push_back(bullet);
			bullet->setCenter(center());
		}
		else if (!((int)attackAni1->currentFrame == 6))
			isAttacked2 = false;
	}
}

Knight::Knight(KnightType type)
{
	scanRange = 6000;
	isFollowing = true;

	if (type == Skull)
	{
		this->type = SKULLKNIGHT;
		addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/skeleton/walk", 19));
		rect = animationMap[WALK]->rect;
		addChild(animationMap[ATTACK1] = new MultiDirAnimation(this, "image/monster/skeleton/attack", 8));
		animationMap[ATTACK1]->visible = false;
		addChild(animationMap[DEAD] = new MultiDirAnimation(this, "image/monster/skeleton/die", 8, 10, false));
		animationMap[DEAD]->visible = false;
		speed = 150;

		coliderPivot = Vec2(187, 224);
		coliderRange = 22;
		attackRange = 50;

		maxHp = hp = 10;
		power = 3;

		attackFrame1 = 5;
	}
	else if (type == Bronze)
	{
		this->type = BRONZEKNIGHT;
		addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/bronze/walk", 11));
		rect = animationMap[WALK]->rect;
		addChild(animationMap[ATTACK1] = new MultiDirAnimation(this, "image/monster/bronze/attack", 8));
		animationMap[ATTACK1]->visible = false;
		addChild(animationMap[DEAD] = new MultiDirAnimation(this, "image/monster/bronze/die", 6, 10, false));
		animationMap[DEAD]->visible = false;
		speed = 170;

		coliderPivot = Vec2(263, 324);
		coliderRange = 36;
		attackRange = 120;

		maxHp = hp = 20;
		power = 5;

		attackFrame1 = 5;
	}
	else if (type == Silver)
	{
		this->type = SILVERKNIGHT;
		addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/silver/walk", 11));
		rect = animationMap[WALK]->rect;
		addChild(animationMap[ATTACK1] = new MultiDirAnimation(this, "image/monster/silver/attack", 8));
		animationMap[ATTACK1]->visible = false;
		addChild(animationMap[DEAD] = new MultiDirAnimation(this, "image/monster/silver/die", 6, 10, false));
		animationMap[DEAD]->visible = false;
		speed = 180;

		coliderPivot = Vec2(263, 324);
		coliderRange = 36;
		attackRange = 120;

		maxHp = hp = 35;
		power = 7;

		attackFrame1 = 5;
	}
	else if (type == Gold)
	{
		this->type = GOLDKNIGHT;
		addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/monster/gold/walk", 11));
		rect = animationMap[WALK]->rect;
		addChild(animationMap[ATTACK1] = new MultiDirAnimation(this, "image/monster/gold/attack", 8));
		animationMap[ATTACK1]->visible = false;
		addChild(animationMap[DEAD] = new MultiDirAnimation(this, "image/monster/gold/die", 6, 10, false));
		animationMap[DEAD]->visible = false;
		speed = 190;

		coliderPivot = Vec2(263, 324);
		coliderRange = 36;
		attackRange = 120;

		maxHp = hp = 50;
		power = 10;

		attackFrame1 = 5;
	}
}

Knight::~Knight() { };

void Knight::Attack()
{
	ChangeState(ATTACK1);
	GameScene *scene = (GameScene*)world.currentScene;
	MultiDirAnimation *attackAni1 = (MultiDirAnimation*)animationMap[ATTACK1];

	if ((int)attackAni1->currentFrame == attackFrame1 && !isAttacked1 && !scene->player->isAvoiding)
	{
		scene->player->GetDamage(power, false);
		isAttacked1 = true;
	}
	else if (!((int)attackAni1->currentFrame == attackFrame1))
		isAttacked1 = false;
}