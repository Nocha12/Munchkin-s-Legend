#include "DXUT.h"
#include "GameScene.h"
#include "Monster.h"
#include "Boss.h"
#include "GameManager.h"
#include "World.h"
#include "StageSelectScene.h"
#include "MainScene.h"

GameScene::GameScene(int stage) : stage(stage), minute(2), second(0), timer(0), isStageTextFaidIn(true), isFinish(false), isClear(false), isViewResult(false), finishTimer(0)
{
	SetStage();
	ui = new UI(stage);
	if (stage == 3)
		minute = 0;
	addChild(dropBox = new DropBox(stage));
}

GameScene::~GameScene()
{
	delete(ui);
}

void GameScene::update(float dt)
{
	ui->update(dt);

	if (isFinish)
	{
		if (isClear)
		{
			if (ui->visible && world.getKeyState(VK_RETURN) == -1)
				ui->ed->stop = false;
			if (ui->ed->currentFrame >= ui->ed->textures.size() - 1)
			{
				world.changeScene(new MainScene());
				return;
			}

			if (ui->ed->visible)
				return;

			if (finishTimer == 0)
			{
				dropBox->setCenter(player->ColPivotWithPos() - Vec2(0, 100));
				dropBox->ani->stop = false;
			}

			finishTimer += dt;

			if (finishTimer > 8)
			{
				if (!isViewResult)
				{
					isViewResult = true;
					resultScaleTimer.reset(0.01f, 100);
					resultScaleTimer.onTick = [=]()
					{
						ui->resultClear->scale = lerp(ui->resultClear->scale, Vec2(1, 1), (float)resultScaleTimer.count.first / 100);
					};
				}

				if (ui->resultClear->scale.x > 0.8f && world.getKeyState(VK_RETURN) == 1)
				{
					if (stage == 3)
					{
						ui->ed->visible = true;
					}
					else
					{
						gameManager.stage = stage + 1;
						world.changeScene(new StageSelectScene());
					}
				}
				return;
			}
		}
		else {
			if (!isViewResult)
			{
				isViewResult = true;
				resultScaleTimer.reset(0.01f, 100);
				resultScaleTimer.onTick = [=]()
				{
					ui->resultFail->scale = lerp(ui->resultFail->scale, Vec2(1, 1), (float)resultScaleTimer.count.first / 100);
				};
			}

			if (ui->resultFail->scale.x > 0.8f && world.getKeyState(VK_RETURN) == 1)
				world.changeScene(new StageSelectScene());

			return;
		}
	}

	Scene::update(dt);

	if (isStageTextFaidIn && ui->stageText->color.a < 1)
		ui->stageText->color.a += 0.01f;
	if (ui->stageText->color.a >= 1)
		isStageTextFaidIn = false;
	if (!isStageTextFaidIn && ui->stageText->color.a > 0)
		ui->stageText->color.a -= 0.01f;

	for (int i = 0; i < 10; ++i)
		if (ui->hpPotionText[i]->visible)
			ui->hpPotionText[i]->visible = false;
	ui->hpPotionText[gameManager.hpPotionCount]->visible = true;
	for (int i = 0; i < 10; ++i)
		if (ui->mpPotionText[i]->visible)
			ui->mpPotionText[i]->visible = false;
	ui->mpPotionText[gameManager.mpPotionCount]->visible = true;

	int goldT = gameManager.gold;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		if (ui->goldText[2 - i][j]->visible)
			ui->goldText[2 - i][j]->visible = false;

		ui->goldText[2 - i][goldT % 10]->visible = true;
		goldT /= 10;
	}

	int secondT = second;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		if (ui->timerText[2 - i][j]->visible)
			ui->timerText[2 - i][j]->visible = false;

		if (i == 2)
			ui->timerText[0][minute]->visible = true;
		else
		{
			ui->timerText[2 - i][secondT % 10]->visible = true;
			secondT /= 10;
		}
	}

	if (!isFinish)
		timer += dt;

	if (timer >= 1)
	{
		timer = 0;
		second--;
		if (second < 0 && minute >= 1)
		{
			second = 59;
			minute--;
		}
		else if (second < 0)
			second = 0;
	}

	for (int i = 0; i < 6; i++)
	{
		if (player->mana > i)
			ui->mpBall[i]->visible = true;
		else 
			ui->mpBall[i]->visible = false;
	}

	if (minute == 0 && second == 0 && stage != 3)
		isFinish = true;

	if (pos.x > -50)
		pos.x = -50;
	if (pos.x < -3500 + offset.x * 2)
		pos.x = -3500 + offset.x * 2;
	if (pos.y > -50)
		pos.y = -50;
	if (pos.y < -1705 + offset.y * 2)
		pos.y = -1705 + offset.y * 2;

	for_iter(bullet, bulletList)
	{
		if (collision(player->center(), (*bullet)->center(), player->coliderRange + (*bullet)->range) && !player->isAvoiding)
		{
			player->GetDamage((*bullet)->damage);
			(*bullet)->removed = true;
		}
	}

	for_iter(dust, dustList)
	{
		(*dust)->scale -= Vec2(dt, dt);
		(*dust)->rotation += dt;

		if ((*dust)->scale.x < 0)
		{
			removeChild(*dust);
			dust = dustList.erase(dust);
			if (dust == dustList.end())
				break;
		}
	}

	for_iter(effect, effectList)
	{
		if ((*effect)->scale.x >= 0.35f)
		{
			(*effect)->scale += Vec2(dt * 1, dt * 1);
			(*effect)->color.a -= 0.05f;
		}
		else
			(*effect)->scale += Vec2(dt * 10, dt * 10);

		if ((*effect)->color.a <= 0)
		{
			removeChild(*effect);
			effect = effectList.erase(effect);
			if (effect == effectList.end())
				break;
		}
	}

	for_iter(effect, hpMpEffectList)
	{
		(*effect)->color.a -= 0.01f;
		(*effect)->pos.y -= 60 * dt;

		if ((*effect)->color.a <= 0)
		{
			removeChild(*effect);
			effect = hpMpEffectList.erase(effect);
			if (effect == hpMpEffectList.end())
				break;
		}
	}

	if (isMapFrontFaidIn)
		mapFront->color.a = mapFront->color.a + dt * 2 < 1 ? mapFront->color.a + dt * 2 : 1;
	else
		mapFront->color.a = mapFront->color.a - dt * 2 > 0.6f ? mapFront->color.a - dt * 2 : 0.6f;
}

void GameScene::render()
{
	Scene::render();
	
	map->render();
	for_iter(iter, magicCircleList)
		(*iter)->render();
	dropBox->render();
	for_iter(iter, itemList)
		(*iter)->render();
	for_iter(iter, afterImageList)
		(*iter)->render();
	for_iter(iter, dustList)
		(*iter)->render();
	unitList.sort([](Unit *obj1, Unit *obj2){
		return (obj1->ColPivotWithPos().y < obj2->ColPivotWithPos().y) ? true : false;
	});
	for_iter(iter, unitList)
		(*iter)->render();
	for_iter(iter, hpMpEffectList)
		(*iter)->render(); 
	for_iter(iter, effectList)
		(*iter)->render();
	for_iter(iter, bulletList)
		(*iter)->render();
	mapFront->render();
	ui->render();
}

void GameScene::SetStage()
{
	if (stage == 1)
	{
		enemyManager = new EnemyManager(1);
		addChild(enemyManager);

		addChild(map = new Sprite("image/map/1/0.png"));
		addChild(mapFront = new Sprite("image/map/1/1.png"));
		player = new Player();
		addChild(player);
		unitList.push_back(player);
		follow(player);
		setCenter(-player->center());
		player->setCenter(Vec2(1775, 888));

		Vec2 treeBottomPos[6] = {
			Vec2(885, 616),
			Vec2(1637, 17),
			Vec2(2634, 444),
			Vec2(2278, 973),
			Vec2(3204, 728),
			Vec2(1713, 1613)
		};

		for (int i = 0; i < 6; i++)
		{
			TreeBottom *treeBottom = new TreeBottom();
			addChild(treeBottom);
			unitList.push_back(treeBottom);
			treeBottom->pos = treeBottomPos[i];
		}

		Vec2 rockPos[6] = {
			Vec2(1214, 349),
			Vec2(694, 718),
			Vec2(794, 1136),
			Vec2(1854, 597),
			Vec2(2570, 454),
			Vec2(2781, 1054)
		};

		for (int i = 0; i < 6; i++)
		{
			Rock *rock = new Rock();
			addChild(rock);
			unitList.push_back(rock);
			rock->pos = rockPos[i];
		}
	} // ------------------------------ stage 1 -------------------------

	if (stage == 2)
	{
		enemyManager = new EnemyManager(2);
		addChild(enemyManager);

		addChild(map = new Sprite("image/map/2/0.png"));
		addChild(mapFront = new Sprite("image/map/2/1.png"));

		player = new Player();
		addChild(player);
		unitList.push_back(player);
		follow(player);
		player->setCenter(Vec2(2405, 540));
		setCenter(-player->center());

		House *house1 = new House(0);
		addChild(house1);
		unitList.push_back(house1);
		house1->pos = Vec2(482, 560);

		House *house2 = new House(1);
		addChild(house2);
		unitList.push_back(house2);
		house2->pos = Vec2(1307, 55);

		Fountain *fountain = new Fountain();
		addChild(fountain);
		unitList.push_back(fountain);
		fountain->pos = Vec2(1550, 659);

		Tree *tree1 = new Tree(0);
		addChild(tree1);
		unitList.push_back(tree1);
		tree1->pos = Vec2(2045, 98);

		Tree *tree2 = new Tree(0);
		addChild(tree2);
		unitList.push_back(tree2);
		tree2->pos = Vec2(664, 400);

		Tree *tree3 = new Tree(1);
		addChild(tree3);
		unitList.push_back(tree3);
		tree3->pos = Vec2(1754, 1318);

		Tree *tree4 = new Tree(1);
		addChild(tree4);
		unitList.push_back(tree4);
		tree4->pos = Vec2(3089, 604);
	} // -------------------stage 2---------------------

	if (stage == 3)
	{
		enemyManager = new EnemyManager(3);
		addChild(enemyManager);

		addChild(map = new Sprite("image/map/3/0.png"));
		addChild(mapFront = new Sprite("image/map/3/1.png"));
		player = new Player();
		addChild(player);
		unitList.push_back(player);
		follow(player);
		setCenter(-player->center());
		player->setCenter(Vec2(1775, 888));

		Vec2 pillarPos[12] = {
			Vec2(1693, -186),
			Vec2(1400, -40),
			Vec2(1113, 103),
			Vec2(829, 244),
			Vec2(545, 386),
			Vec2(259, 529),
			Vec2(3140, 534),
			Vec2(2847, 680),
			Vec2(2560, 823),
			Vec2(2276, 964),
			Vec2(1992, 1106),
			Vec2(1706, 1249)
		};

		for (int i = 0; i < 12; i++)
		{
			Pillar *pillar = new Pillar();
			addChild(pillar);
			unitList.push_back(pillar);
			pillar->pos = pillarPos[i];
		}
	} // ------------------------------ stage 3 -------------------------
}