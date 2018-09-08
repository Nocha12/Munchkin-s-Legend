#include "DXUT.h"
#include "EnemyManager.h"
#include "Monster.h"
#include "World.h"
#include "GameScene.h"

EnemyManager::EnemyManager(int stage) : slimeCount(0), bigSlimeCount(0), skullKnightCount(0), bronzeKnightCount(0), silverKnightCount(0), goldKnightCount(0), isSpawnStarted(false), stage(stage), bossCount(0)
{
	isSpawned[SKULLKNIGHT] = false;
	isSpawned[SILVERKNIGHT] = false;
}

EnemyManager::~EnemyManager()
{
}

void EnemyManager::update(float dt)
{
	Entity::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	if (!isSpawnStarted)
	{
		isSpawnStarted = true;

		for (int i = 0; i < 5; i++)
		{
			++slimeCount;
			SmallSlime *slime = new SmallSlime();
			scene->addChild(slime);
			scene->unitList.push_back(slime);
			slime->setCenter(RandomPos());
		}

		slimeRespawnTimer.reset(5, 5);
		slimeRespawnTimer.onTick = [=](){
			++slimeCount;
			SmallSlime *slime = new SmallSlime();
			scene->addChild(slime);
			scene->unitList.push_back(slime);
			slime->setCenter(RandomPos());
		};
		bigSlimeRespawnTimer.reset(30, 3);
		bigSlimeRespawnTimer.onTick = [=]() {
			++bigSlimeCount;
			BigSlime *slime = new BigSlime();
			scene->addChild(slime);
			scene->unitList.push_back(slime);
			slime->setCenter(RandomPos());
		};

		if (stage == 3)
		{
			sitBoss = new SitBoss();
			scene->addChild(sitBoss);
			scene->unitList.push_back(sitBoss);
			sitBoss->setCenter(Vec2(2121, -95));

			skullKnightRespawnTimer.reset(60, 1);
			skullKnightRespawnTimer.onTick = [=]() {
				for (int i = 0; i < 5; i++)
				{
					++skullKnightCount;
					Knight *knight = new Knight(Skull);
					scene->addChild(knight);
					scene->unitList.push_back(knight);
					knight->setCenter(RandomPos());
				}
			};
			bronzeKnightRespawnTimer.reset(90, 1);
			bronzeKnightRespawnTimer.onTick = [=]() {
				for (int i = 0; i < 3; i++)
				{
					++bronzeKnightCount;
					Knight *knight = new Knight(Bronze);
					scene->addChild(knight);
					scene->unitList.push_back(knight);
					knight->setCenter(RandomPos());
				}
			};
		}
	}

	if (stage == 2 && !bigSlimeRespawnTimer.active && bigSlimeCount == 0 && !isSpawned[SKULLKNIGHT])
	{
		for (int i = 0; i < 3; i++)
		{
			++skullKnightCount;
			Knight *knight = new Knight(Skull);
			scene->addChild(knight);
			scene->unitList.push_back(knight);
			knight->setCenter(RandomPos());
		}
		isSpawned[SKULLKNIGHT] = true;
	}
	if (stage == 3 && !bronzeKnightRespawnTimer.active && bronzeKnightCount == 0 && !isSpawned[SILVERKNIGHT])
	{
		for (int i = 0; i < 2; i++)
		{
			++silverKnightCount;
			Knight *knight = new Knight(Silver);
			scene->addChild(knight);
			scene->unitList.push_back(knight);
			knight->setCenter(RandomPos());
		}
		++goldKnightCount;
		Knight *knight = new Knight(Gold);
		scene->addChild(knight);
		scene->unitList.push_back(knight);
		knight->setCenter(RandomPos());

		isSpawned[SILVERKNIGHT] = true;
	}

	if (stage == 3 && !bronzeKnightRespawnTimer.active && silverKnightCount == 0 && goldKnightCount == 0 && bigSlimeCount == 0 && slimeCount == 0 && isSpawned[SILVERKNIGHT] && !isSpawned[BOSS])
	{
		scene->unitList.remove(sitBoss);
		removeChild(sitBoss);
		++bossCount;
		Boss *boss = new Boss();
		scene->addChild(boss);
		scene->unitList.push_back(boss);
		boss->pos = Vec2(2121, -95);

		isSpawned[BOSS] = true;
	}

	if (!bigSlimeRespawnTimer.active && slimeCount == 0 && bigSlimeCount == 0 && skullKnightCount == 0 && bronzeKnightCount == 0 && silverKnightCount == 0 && goldKnightCount == 0 && bossCount == 0)
	{
		scene->isFinish = true;
		scene->isClear = true;
	}
}

Vec2 EnemyManager::RandomPos()
{
	Vec2 spawnPos = Vec2(random(0, 3550), random(0, 1775));

	while (
		!(spawnPos.y > -(888 / 1775.0f) * spawnPos.x + 950 &&
		spawnPos.y > (888 / 1775.0f) * spawnPos.x - 830 &&
		spawnPos.y < -(888 / 1775.0f) * spawnPos.x + 2604 &&
		spawnPos.y < (888 / 1775.0f) * spawnPos.x + 810) ||
		collision(spawnPos, ((GameScene*)world.currentScene)->player->ColPivotWithPos(), 1000))
	{
		spawnPos = Vec2(random(0, 3550), random(0, 1775));
	}

	return spawnPos;
}