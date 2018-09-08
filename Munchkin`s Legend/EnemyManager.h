#pragma once
#include "Entity.h"
#include "Timer.h"
#include "Boss.h"

class EnemyManager : public Entity
{
public:
	EnemyManager(int stage);
	~EnemyManager();

	Timer slimeRespawnTimer;
	Timer bigSlimeRespawnTimer;
	Timer skullKnightRespawnTimer;
	Timer bronzeKnightRespawnTimer;

	int stage;

	bool isSpawnStarted;
	map<UnitType, bool> isSpawned;

	int slimeCount;
	int bigSlimeCount;
	int skullKnightCount;
	int bronzeKnightCount;
	int silverKnightCount;
	int goldKnightCount;
	int bossCount;

	Vec2 RandomPos();
	void update(float dt);

	SitBoss *sitBoss;
};

