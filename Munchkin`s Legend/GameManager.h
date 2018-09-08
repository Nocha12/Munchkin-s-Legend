#pragma once
class GameManager
{
public:
	GameManager();
	~GameManager();

	void Reset();

	int stage;
	int hpPotionCount;
	int mpPotionCount;
	int hpUpgradeCount;
	int powerUpgradeCount;
	int gold;

	bool canUseSkillD;
	bool useExtraMana;
};

extern GameManager gameManager;
