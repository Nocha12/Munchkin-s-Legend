#include "DXUT.h"
#include "Player.h"
#include "World.h"
#include "GameScene.h"
#include "GameManager.h"

Player::Player() : attackCount(0), isAvoiding(false), isUsingSkill(false), isDashing(false), dashTimer(0), shildUp(0), isPushingShield(false), speedOffset(5), isBuff(false), isBinding(false), dustTimer(0), mana(5), manaTimer(0), isGod(false)
{
	type = PLAYER;
	addChild(animationMap[STOP] = new MultiDirAnimation(this, "image/player/stop", 8));
	rect = animationMap[STOP]->rect;
	addChild(animationMap[WALK] = new MultiDirAnimation(this, "image/player/walk", 7, 8));
	animationMap[WALK]->visible = false;
	addChild(animationMap[RUN] = new MultiDirAnimation(this, "image/player/run", 12, 12));
	animationMap[RUN]->visible = false;
	addChild(animationMap[AVOID] = new MultiDirAnimation(this, "image/player/avoid", 5, 10));
	animationMap[AVOID]->visible = false;
	addChild(animationMap[ATTACK1] = new MultiDirAnimation(this, "image/player/attack/0", 10, 10, false));
	animationMap[ATTACK1]->visible = false;
	addChild(animationMap[ATTACK2] = new MultiDirAnimation(this, "image/player/attack/1", 10, 10, false));
	animationMap[ATTACK2]->visible = false;
	addChild(animationMap[DashSkillStart] = new MultiDirAnimation(this, "image/player/skill1/attack", 6, 40, false));
	animationMap[DashSkillStart]->visible = false;
	addChild(animationMap[DashSkillFinish] = new MultiDirAnimation(this, "image/player/skill1/finish", 4, 20, false));
	animationMap[DashSkillFinish]->visible = false;
	addChild(animationMap[ShildUp] = new MultiDirAnimation(this, "image/player/skill2/up", 8, 10, false));
	animationMap[ShildUp]->visible = false;
	addChild(animationMap[ShildDown] = new MultiDirAnimation(this, "image/player/skill2/down", 3, 10, false));
	animationMap[ShildDown]->visible = false;
	((MultiDirAnimation*)animationMap[DashSkillFinish])->Stop();
	addChild(animationMap[BuffSkill] = new MultiDirAnimation(this, "image/player/skill3", 11, 10, false));
	animationMap[BuffSkill]->visible = false;
	speed = 250;
	hp = maxHp = 25 + gameManager.hpUpgradeCount * 25;

	attackFrame1 = 6;
	attackFrame2 = 6;

	coliderPivot = center() + Vec2(0, 30);
	coliderRange = 28;
	attackRange = 110;
	power = 2 + gameManager.powerUpgradeCount;
	addChild(bindAni = new Animation("image/effect/mob/boss", 8, 15, false, true));
	bindAni->stop = true;
	bindAni->setCenter(coliderPivot);

	addChild(buffSkilleffect = new Animation("image/effect/player/skill2", 18, 15, false, true));
	buffSkilleffect->stop = true;
	buffSkilleffect->visible = false;
	buffSkilleffect->setCenter(rect.center());
}

Player::~Player()
{
}

void Player::update(float dt)
{
	GameScene *scene = (GameScene*)world.currentScene;

	scene->ui->hpBar->visibleRect.right = scene->ui->hpBar->rect.right * hp / maxHp;

	if (world.getKeyState(VK_F1) == 1)
	{
		if (isGod)
			isGod = false;
		else
			isGod = true;
	}

	if (isGod)
	{
		hp = maxHp;
		power = 10;
	}

	if (hp <= 0)
	{
		hp = 0;
		scene->isFinish = true;
	}

	manaTimer += dt;
	if (manaTimer > 8)
	{
		if (gameManager.useExtraMana)
			mana = mana + 1 > 6 ? 6 : mana + 1;
		else
			mana = mana + 1 > 5 ? 5 : mana + 1;
		manaTimer = 0;
	}

	if (isBinding && bindAni->stop)
	{
		bindAni->stop = false;
		bindAni->color.a = 1;
		bindFaidOutTimer.reset(0.05f, 30);
		bindFaidOutTimer.onTick = [=](){
			bindAni->color.a -= 1 / 30.0f;
		};
		bindFaidOutTimer.onFinished = [=](){
			bindAni->stop = true;
			bindAni->currentFrame = 0;
		};
	}

	if (!isSetMinimapImg)
	{
		scene->ui->addChild(minimapImg = new Sprite("image/UI/minimap/player.png"));
		isSetMinimapImg = true;
	}

	Unit::update(dt);

	MultiDirAnimation *attackAni1 = (MultiDirAnimation*)animationMap[ATTACK1];
	MultiDirAnimation *attackAni2 = (MultiDirAnimation*)animationMap[ATTACK2];

	bool isMoving = false;
	
	int mapFrontRangeLeft, mapFrontRangeRight;

	if (scene->stage == 1)
	{
		mapFrontRangeLeft = 2404;
		mapFrontRangeRight = 610;
	}
	else if (scene->stage == 2)
	{
		mapFrontRangeLeft = 2304;
		mapFrontRangeRight = 570;
	}
	else if (scene->stage == 3)
	{
		mapFrontRangeLeft = 2304;
		mapFrontRangeRight = 570;
	}

	if (ColPivotWithPos().y > -(888 / 1775.0f) * ColPivotWithPos().x + mapFrontRangeLeft)
		scene->isMapFrontFaidIn = false;
	else if (ColPivotWithPos().y > (888 / 1775.0f) * ColPivotWithPos().x + mapFrontRangeRight)
		scene->isMapFrontFaidIn = false;
	else
		scene->isMapFrontFaidIn = true;

	if (world.getKeyState('1') == 1 && gameManager.hpPotionCount > 0)
	{
		gameManager.hpPotionCount--;
		hp = hp + 20 > maxHp ? maxHp : hp + 20;
		hpPotionEffectTimer.reset(0.2f, 5);
		hpPotionEffectTimer.onTick = [=](){
			Sprite *img = new Sprite("image/UI/potionHp.png");
			scene->addChild(img);
			scene->hpMpEffectList.push_back(img);
			img->setCenter(ColPivotWithPos() + Vec2(random(-25, 25), random(20, 50)));
		};
	}
	if (world.getKeyState('2') == 1 && gameManager.mpPotionCount > 0)
	{
		gameManager.mpPotionCount--;
		if (gameManager.useExtraMana)
			mana = 6;
		else
			mana = 5;
		mpPotionEffectTimer.reset(0.2f, 5);
		mpPotionEffectTimer.onTick = [=](){
			Sprite *img = new Sprite("image/UI/potionMp.png");
			scene->addChild(img);
			scene->hpMpEffectList.push_back(img);
			img->setCenter(ColPivotWithPos() + Vec2(random(-25, 25), random(20, 50)));
		};
	}

	if (world.getKeyState('Z') == 1 && shildUp == 0) // attack
	{
		if (attackCount == 0)
		{
			ChangeState(ATTACK1, true);
			attackCount = 1;
		}
		else if (attackCount == 1)
			attackCount = 2;
	}

	if (!attackAni1->isPlaying)
	{
		if (attackCount == 2)
		{
			ChangeState(ATTACK2, true);
			attackCount = 3;
		}
		else if (attackCount != 3)
			attackCount = 0;
	}

	if (!attackAni2->isPlaying && attackCount == 3)
	{
		attackCount = 0;
	}  // attack 끗

	if (world.getKeyState('X') == 2 && attackCount == 0) // 방어
	{
		ChangeState(ShildUp);
		shildUp = 1;
	}
	else if (shildUp == 1)
	{
		ChangeState(ShildDown, true);
		shildUp = 2;
	}
	else if (shildUp == 2 && !((MultiDirAnimation*)animationMap[ShildDown])->isPlaying && !isPushingShield)
	{
		shildUp = 0;
	}

	if (world.getKeyState('C') == 1 && !isAvoiding && !isUsingSkill && !isBinding && mana >= 1) // 회피
	{
		mana--;
		attackCount = 0;
		afterImageTimer.reset(0.0375f, 7);
		isAvoiding = true;
		ChangeState(AVOID, true);
		afterImageTimer.onTick = [=]()
		{
			MultiDirAnimation *avoidAnimation = (MultiDirAnimation*)animationMap[AVOID];
			Sprite *sprite = new Sprite();
			sprite->setTexture(avoidAnimation->animation[avoidAnimation->direction]->texture);
			AfterImage *img = new AfterImage(sprite);
			world.currentScene->addChild(img);
			scene->afterImageList.push_back(img);
			img->pos = pos; 
		};
		afterImageTimer.onFinished = [=]()
		{
			isAvoiding = false;
		};
	}

	if (isAvoiding)
		pos += Vec2(cosf(moveRotation - D3DXToRadian(180)), sinf(moveRotation - D3DXToRadian(180))) * speed * 2.5f * dt;

	if (world.getKeyState('A') == 1 && !isAvoiding && !isUsingSkill && shildUp == 1 && mana >= 3) // 방패밀치기 스킬
	{
		mana -= 2;
		isUsingSkill = true;
		attackCount = 0;
		isPushingShield = true;

		afterImageTimer.reset(0.05f, 6);

		afterImageTimer.onTick = [=]()
		{
			MultiDirAnimation *skillAnimation = (MultiDirAnimation*)animationMap[ShildUp];
			Sprite *sprite = new Sprite();
			sprite->setTexture(skillAnimation->animation[skillAnimation->direction]->texture);
			AfterImage *img = new AfterImage(sprite);
			scene->afterImageList.push_back(img);
			world.currentScene->addChild(img);
			img->pos = pos;
		};
	}

	if (isPushingShield)
	{
		dashTimer += dt;
		pos += Vec2(cosf(moveRotation), sinf(moveRotation)) * speed * speedOffset * dt;
		speedOffset = speedOffset - dt * 15 < 0.5f ? 0.5f : speedOffset - dt * 15;
		for_iter(unit, scene->unitList)
		{
			if (collision(ColPivotWithPos(), (*unit)->ColPivotWithPos(), (*unit)->coliderRange + attackRange) && (*unit)->hp > 0)
			{
				if (*unit == this) continue;

				bool isAttacked = false;

				for_iter(attacked, attackedObjectList)
				{
					if ((*attacked) == (*unit))
					{	
						isAttacked = true;
						break;
					}
				}

				if (!isAttacked)
				{
					(*unit)->GetDamage(power * 5);
					(*unit)->backSpeed = 400;
					attackedObjectList.push_back(*unit);
				}
			}
		}
	}

	if (dashTimer >= 0.5f && isPushingShield)
	{
		speedOffset = 5;
		dashTimer = 0;
		isPushingShield = false;
		isUsingSkill = false;
		attackedObjectList.clear();
	}

	if (world.getKeyState('S') == 1 && !isAvoiding && !isUsingSkill && shildUp == 0 && mana >= 4) // 일섬 스킬
	{
		mana -= 3;
		world.timeScale = 0.2;
		isUsingSkill = true;
		attackCount = 0;

		faidInTimer.reset(0.0005f, 10);	
		faidInTimer.onTick = [=]()
		{
			scene->ui->black->color.a += 0.07f;
		};

		afterImageTimer.reset(0.002f, 20);

		ChangeState(DashSkillStart, true);
		afterImageTimer.onTick = [=]()
		{
			MultiDirAnimation *skillAnimation = (MultiDirAnimation*)animationMap[DashSkillStart];
			Sprite *sprite = new Sprite();
			sprite->setTexture(skillAnimation->animation[skillAnimation->direction]->texture);
			AfterImage *img = new AfterImage(sprite);
			world.currentScene->addChild(img);
			scene->afterImageList.push_back(img);
			img->pos = pos;
		};
		afterImageTimer.onFinished = [=]()
		{
			isDashing = true;
			afterImageTimer.onFinished = [=](){};
		};
	}

	if (isDashing)
	{
		dashTimer += dt;
		pos += Vec2(cosf(moveRotation), sinf(moveRotation)) * speed * 150 * dt;

		for_iter(unit, scene->unitList)
		{
			if (collision(ColPivotWithPos(), (*unit)->ColPivotWithPos(), (*unit)->coliderRange + attackRange) && (*unit)->hp > 0)
			{
				if (*unit == this) continue;

				bool isAttacked = false;

				for_iter(attacked, attackedObjectList)
				{
					if ((*attacked) == (*unit))
					{
						isAttacked = true;
						break;
					}
				}

				if (!isAttacked)
				{
					attackedObjectList.push_back(*unit);
				}
			}
		}
	}

	if (dashTimer >= 0.01f && isDashing)
	{
		faidOutTimer.reset(0.01f, 90);
		faidOutTimer.onTick = [=]()
		{
			scene->ui->black->color.a -= 0.01f;
		};

		dashTimer = 0;
		isDashing = false;
		ChangeState(DashSkillFinish, true);
		afterImageTimer.reset(0.005f, 15);
		afterImageTimer.onTick = [=]()
		{
			MultiDirAnimation *skillAnimation = (MultiDirAnimation*)animationMap[DashSkillFinish];
			Sprite *sprite = new Sprite();
			sprite->setTexture(skillAnimation->animation[skillAnimation->direction]->texture);
			AfterImage *img = new AfterImage(sprite);
			world.currentScene->addChild(img);
			scene->afterImageList.push_back(img);
			img->pos = pos;
		};
		afterImageTimer.onFinished = [=]()
		{
			world.timeScale = 1;
			isUsingSkill = false;

			for_iter(attacked, attackedObjectList)
			{
				for (int i = 0; i < 3; i++)
					(*attacked)->GetDamage(power * 3);
			}
			attackedObjectList.clear();
			afterImageTimer.onFinished = [=](){};
		};
	}

	if (world.getKeyState('D') == 1 && !isAvoiding && !isUsingSkill && shildUp == 0 && !buffTimer.active && mana >= 1 && gameManager.canUseSkillD) // 버프 스킬
	{
		mana--; 
		isBuff = true;
		isUsingSkill = true;
		attackCount = 0;

		buffSkilleffect->stop = false;
		buffSkilleffect->visible = true;
		buffTimer.reset(5, 1);

		ChangeState(BuffSkill, true);
		buffTimer.onTick = [=]()
		{
			power = 6 + gameManager.powerUpgradeCount * 3;
		};
		buffTimer.onFinished = [=]()
		{
			power = 2 + gameManager.powerUpgradeCount;
			buffSkilleffect->stop = true;
			buffSkilleffect->visible = false;
			buffSkilleffect->currentFrame = 0;
		};
	}
	if (isBuff && (int)((MultiDirAnimation*)animationMap[BuffSkill])->currentFrame == 7)
	{
		for_iter(unit, scene->unitList)
		{
			if (*unit == this) continue;

			if (collision(ColPivotWithPos(), (*unit)->ColPivotWithPos(), (*unit)->coliderRange + attackRange + 50) && (*unit)->hp > 0)
				(*unit)->backSpeed = 500;
		}
	}
	if (isBuff && !((MultiDirAnimation*)animationMap[BuffSkill])->isPlaying)
	{
		isBuff = false;
		isUsingSkill = false;
	}

	if (attackCount == 0 && !isAvoiding && !isUsingSkill && shildUp == 0 && backSpeed <= 100 && !isBinding) // 기본 이동
	{
		if (world.getKeyState(VK_UP) > 0)
		{
			isMoving = true;
			moveRotation = D3DXToRadian(-90);
		}
		if (world.getKeyState(VK_DOWN) > 0)
		{
			isMoving = true;
			moveRotation = D3DXToRadian(90);
		}
		if (world.getKeyState(VK_LEFT) > 0)
		{
			isMoving = true;
			moveRotation = D3DXToRadian(180);

			if (world.getKeyState(VK_UP) > 0)
				moveRotation = D3DXToRadian(-135);
			if (world.getKeyState(VK_DOWN) > 0)
				moveRotation = D3DXToRadian(135);
		}
		if (world.getKeyState(VK_RIGHT) > 0)  
		{
			isMoving = true;
			moveRotation = D3DXToRadian(0);

			if (world.getKeyState(VK_UP) > 0)
				moveRotation = D3DXToRadian(-45);
			if (world.getKeyState(VK_DOWN) > 0)
				moveRotation = D3DXToRadian(45);
		}
	}

	if (isBinding)
		for_iter(ani, animationMap)
		((MultiDirAnimation*)ani->second)->GetBlack();

	if ((int)attackAni1->currentFrame == attackFrame1 && !isAttacked1)
	{
		isAttacked1 = true;

		for_iter(unit, scene->unitList)
		{
			if (*unit == this) continue;

			if (collision(ColPivotWithPos(), (*unit)->ColPivotWithPos(), (*unit)->coliderRange + attackRange) && (*unit)->hp > 0)
			{
				(*unit)->GetDamage(power);
			}
		}
	}
	else if (!((int)attackAni1->currentFrame == attackFrame1))
		isAttacked1 = false;

	if ((int)attackAni2->currentFrame == attackFrame2 && !isAttacked2)
	{
		isAttacked2 = true;

		for_iter(unit, scene->unitList)
		{
			if (*unit == this) continue;
			
			if (collision(ColPivotWithPos(), (*unit)->ColPivotWithPos(), (*unit)->coliderRange + attackRange) && (*unit)->hp > 0)
			{
				(*unit)->GetDamage(power);
			}
		}
	}
	else if (!((int)attackAni2->currentFrame == attackFrame2))
		isAttacked2 = false;

	if (isMoving)
	{
		if (world.getKeyState(VK_LSHIFT) == 2)
		{
			pos += Vec2(cosf(moveRotation), sinf(moveRotation)) * speed * 1.4f * dt;
			ChangeState(RUN);

			dustTimer += dt;

			if (dustTimer > 0.12f)
			{
				dustTimer = 0;
				Sprite *dust = new Sprite("image/UI/dust.png");
				scene->addChild(dust);
				dust->rotationCenter = dust->rect.center();
				dust->setCenter(ColPivotWithPos());
				scene->dustList.push_back(dust);
			}
		}
		else {
			pos += Vec2(cosf(moveRotation), sinf(moveRotation)) * speed * dt;
			ChangeState(WALK);
		}
	}
	else if (attackCount == 0 && !isAvoiding && !isUsingSkill && shildUp == 0)
		ChangeState(STOP);

	for_iter(item, scene->itemList)
	{
		if (collision(center(), (*item)->center(), 40))
		{
			if ((*item)->type == GoldItem)
				gameManager.gold += 1;
			if ((*item)->type == HpPotion)
				gameManager.hpPotionCount = gameManager.hpPotionCount + 1 > 9 ? 9 : gameManager.hpPotionCount + 1;
			if ((*item)->type == MpPotion)
				gameManager.mpPotionCount = gameManager.mpPotionCount + 1 > 9 ? 9 : gameManager.mpPotionCount + 1;
			if ((*item)->type == Necklace)
			{
				gameManager.canUseSkillD = true;
				scene->ui->skillD->visible = true;
			}
			if ((*item)->type == Ring)
			{
				gameManager.useExtraMana = true;
				scene->ui->emptyMpBall->visible = true;
			}
			removeChild(*item);
			item = scene->itemList.erase(item);
			if (item == scene->itemList.end())
				break;
		}
	}
}
