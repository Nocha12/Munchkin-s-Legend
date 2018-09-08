#include "DXUT.h"
#include "GameManager.h"

GameManager::GameManager() : stage(1), hpPotionCount(0), mpPotionCount(0), hpUpgradeCount(0), powerUpgradeCount(0), gold(0), canUseSkillD(false), useExtraMana(false)
{ 
}

GameManager::~GameManager()
{
}

void GameManager::Reset()
{
	stage = 1;
	hpPotionCount = 0;
	mpPotionCount = 0; 
	hpUpgradeCount = 0;
	powerUpgradeCount = 0;
}

GameManager gameManager;
