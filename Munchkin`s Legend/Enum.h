#pragma once

enum StateAnimationType
{
	STOP,
	WALK,
	ATTACK1,
	ATTACK2,
	DEAD,
	RUN,
	AVOID,
	BuffSkill,
	DashSkillStart,
	DashSkillFinish,
	ShildUp,
	ShildDown,
	BindSkill
};

enum BulletType
{
	SlimeBullet,
	BossBullet
};

enum UnitType
{
	PLAYER,
	SLIME,
	BIGSLIME,
	SKULLKNIGHT,
	BRONZEKNIGHT,
	SILVERKNIGHT,
	GOLDKNIGHT,
	BOSS
};

enum KnightType
{
	Skull,
	Bronze,
	Silver,
	Gold
};

enum ItemType
{
	GoldItem,
	HpPotion,
	MpPotion,
	Necklace,
	Ring
};