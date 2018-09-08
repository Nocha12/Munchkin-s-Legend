#pragma once
#include "Unit.h"
#include "MultiDirAnimation.h"
#include "Timer.h"

class Player :
	public Unit
{
public:
	Player();
	~Player();
	
	Animation *bindAni;
	Animation *buffSkilleffect;

	Timer afterImageTimer;
	Timer bindFaidOutTimer;
	Timer buffTimer;
	Timer hpPotionEffectTimer;
	Timer mpPotionEffectTimer;
	Timer faidInTimer;
	Timer faidOutTimer;

	int attackCount;
	int mana;
	float manaTimer;
	bool isAvoiding;
	bool isUsingSkill;
	bool isDashing;
	bool isBuff;
	bool isPushingShield;
	bool isBinding;
	int shildUp;
	float dashTimer;
	float dustTimer;
	float speedOffset;
	bool isGod;

	list<Unit*> attackedObjectList;

	void update(float dt);
};

