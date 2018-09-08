#pragma once
#include "Scene.h"
#include "Unit.h"
#include "Player.h"
#include "Bullet.h"
#include "AfterImage.h"
#include "MagicCircle.h" 
#include "EnemyManager.h"
#include "UI.h"
#include "Item.h"
#include "DropBox.h"

class GameScene :
	public Scene
{
public:
	GameScene(int stage);
	~GameScene();

	list<Unit*> unitList;
	list<Bullet*> bulletList;
	list<AfterImage*> afterImageList;
	list<MagicCircle*> magicCircleList;
	list<Item*> itemList;
	list<Sprite*> dustList;
	list<Sprite*> effectList;
	list<Sprite*> hpMpEffectList;

	Timer resultScaleTimer;
	Sprite *map;
	Sprite *mapFront;
	Player *player;
	EnemyManager *enemyManager;
	UI *ui;

	DropBox *dropBox;

	bool isMapFrontFaidIn;
	int stage;
	bool isStageTextFaidIn;

	bool isFinish;
	bool isClear;
	bool isViewResult;

	int minute;
	int second;
	float timer;
	float finishTimer;

	void update(float dt);
	void render();

	void SetStage();
};