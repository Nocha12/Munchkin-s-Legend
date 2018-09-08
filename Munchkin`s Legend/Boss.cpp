#include "DXUT.h"
#include "Boss.h"
#include "World.h"
#include "GameScene.h"
#include "MagicCircle.h"

SitBoss::SitBoss() : isAttacked(false)
{
	isMap = true;
	addChild(sitAni = new Animation("image/boss/sitIdle", 10));
	addChild(attackAni = new Animation("image/boss/sitAttack", 10, 15, false));
	attackAni->visible = false;
	attackAni->stop = true;
	pivot = Vec2(407, 453);
}

SitBoss::~SitBoss()
{

}

void SitBoss::update(float dt)
{
	Entity::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	if (collision(scene->player->ColPivotWithPos(), pos + pivot, 250 + scene->player->coliderRange))
	{
		attackAni->visible = true;
		attackAni->stop = false;
		sitAni->visible = false;
	}

	if ((int)attackAni->currentFrame == 5 && !isAttacked)
	{
		scene->player->moveRotation = angle(scene->player->ColPivotWithPos(), pos + pivot);
		scene->player->backSpeed = 500;
		isAttacked = true;
	}
	else if (!((int)attackAni->currentFrame == 3))
		isAttacked = false;

	if (attackAni->currentFrame >= attackAni->textures.size() - 1)
	{
		sitAni->visible = true;
		sitAni->currentFrame = 0;
		attackAni->visible = false;
		attackAni->currentFrame = 0;
		attackAni->stop = true;
	}
}

Boss::Boss() : backDashState(0), backDashTimer(0), delay(0)
{
	type = BOSS;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/boss/idle", 10));
	rect = animationMap[WALK]->rect;
	addChild(animationMap[ATTACK1] = new MultiDirAnimation(this, "image/boss/attack", 10, 6));
	animationMap[ATTACK1]->visible = false;
	((MultiDirAnimation*)animationMap[ATTACK1])->Stop();
	addChild(animationMap[ATTACK2] = new MultiDirAnimation(this, "image/boss/skill2", 7, 10, false));
	animationMap[ATTACK2]->visible = false;
	((MultiDirAnimation*)animationMap[ATTACK2])->Stop();
	addChild(animationMap[BindSkill] = new MultiDirAnimation(this, "image/boss/skill1", 10, 10, false));
	animationMap[BindSkill]->visible = false;
	((MultiDirAnimation*)animationMap[BindSkill])->Stop();
	addChild(animationMap[DEAD] = new MultiDirAnimation(this, "image/boss/die", 7, 10, false));
	animationMap[DEAD]->visible = false;
	speed = 220;

	coliderPivot = Vec2(407, 462);

	coliderRange = 80;
	attackRange = 230;
	isStand = true;

	maxHp = hp = 200;
	power = 25;

	attackFrame1 = 5;
	isFollowing = true;
}

Boss::~Boss()
{
}

void Boss::Attack()
{
	isFollowing = false;

	ChangeState(ATTACK1);
}

void Boss::update(float dt)
{
	MultiDirAnimation *attackAni1 = (MultiDirAnimation*)animationMap[ATTACK1];
	MultiDirAnimation *attackAni2 = (MultiDirAnimation*)animationMap[ATTACK2];
	GameScene *scene = (GameScene*)world.currentScene;

	backSpeed = 0;
	Monster::update(dt);

	if ((int)attackAni1->currentFrame == attackFrame1 && !isAttacked1)
	{
		backDashState = 1;
		if (collision(ColPivotWithPos(), scene->player->ColPivotWithPos(), scene->player->coliderRange + attackRange) && !scene->player->isAvoiding)
		{
			scene->player->moveRotation = angle(scene->player->ColPivotWithPos(), ColPivotWithPos());
			scene->player->backSpeed = 500;
			scene->player->GetDamage(power, false);
		}

		isAttacked1 = true;
	}
	else if (!((int)attackAni1->currentFrame == attackFrame1))
		isAttacked1 = false;

	if (attackAni1->currentFrame >= attackAni1->animation[0]->textures.size() - 1)
	{
		ChangeState(WALK, true);
		if (backDashState == 1) backDashState = 2;
	}

	if (backDashState == 2)
	{
		afterImageTimer.reset(0.1f, 18);
		afterImageTimer.onTick = [=]()
		{
			MultiDirAnimation *walkAnimation = (MultiDirAnimation*)animationMap[WALK];
			Sprite *sprite = new Sprite();
			sprite->setTexture(walkAnimation->animation[walkAnimation->direction]->texture);
			AfterImage *img = new AfterImage(sprite);
			world.currentScene->addChild(img);
			scene->afterImageList.push_back(img);
			img->pos = pos;
		};

		backDashState = 3;
	}

	if (backDashState == 3 && backDashTimer < 1)
	{
		backDashTimer += dt;
		pos -= Vec2(cos(moveRotation), sin(moveRotation)) * 250 * dt;
		canAttack = false;
	}
	else if (backDashState == 3)
	{
		backDashState = 4;
		backDashTimer = 0;
		ChangeState(BindSkill, true);

		MagicCircle *magicCircle = new MagicCircle();
		scene->addChild(magicCircle);
		magicCircle->setCenter(scene->player->ColPivotWithPos());
		scene->magicCircleList.push_back(magicCircle);
	}

	if (!((MultiDirAnimation*)animationMap[BindSkill])->isPlaying && backDashState == 4)
	{
		ChangeState(ATTACK2, true);
		backDashState = 5;

		for (int i = 0; i < 3; i++)
		{
			Bullet *bullet = new Bullet(BossBullet, angle(center(), scene->player->center()) + D3DXToRadian(20 * (i - 1)), -200, 8, power - 5);
			scene->addChild(bullet);
			scene->bulletList.push_back(bullet);
			bullet->setCenter(center());
		}
	}

	if ((int)attackAni2->currentFrame == 4 && !isAttacked2)
	{
		isAttacked2 = true;

		for (int i = 0; i < 4; i++)
		{
			Bullet *bullet = new Bullet(BossBullet, angle(center(), scene->player->center()) + D3DXToRadian(20 * i) - D3DXToRadian(30), 50, 8, power - 5);
			scene->addChild(bullet);
			scene->bulletList.push_back(bullet);
			bullet->setCenter(center());
		}
	}
	else if (!((int)attackAni2->currentFrame == 4))
		isAttacked2 = false;

	if (!((MultiDirAnimation*)animationMap[ATTACK2])->isPlaying && backDashState == 5)
		ChangeState(WALK);

	if (backDashState == 5)
		delay += dt;

	if (delay >= 5)
	{
		canAttack = true;
		delay = 0;
		isFollowing = true;
		backDashState = 0;
	}
}