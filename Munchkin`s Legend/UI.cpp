#include "DXUT.h"
#include "UI.h"
#include "GameManager.h"

UI::UI(int stage)
{
	Sprite *ingame = new Sprite("image/UI/ingame.png");
	addChild(ingame);

	addChild(skillD = new Sprite("image/UI/skillD.png"));
	skillD->pos = Vec2(184, 624);
	skillD->visible = false;
	if (gameManager.canUseSkillD)
		skillD->visible = true;

	addChild(emptyMpBall = new Sprite("image/UI/mp2.png"));
	emptyMpBall->pos = Vec2(282, 70);
	emptyMpBall->visible = false;
	if (gameManager.useExtraMana)
		emptyMpBall->visible = true;

	if (stage == 1)
		addChild(stageText = new Sprite("image/UI/text/1stage.png"));
	if (stage == 2)
		addChild(stageText = new Sprite("image/UI/text/2stage.png"));
	if (stage == 3)
		addChild(stageText = new Sprite("image/UI/text/3stage.png"));

	stageText->color.a = 0;
	stageText->setCenter(Vec2(1280, 720) / 2);

	addChild(hpBar = new Sprite("image/UI/hpBar.png"));
	hpBar->pos = Vec2(89, 33);

	for (int i = 0; i < 6; i++)
	{
		addChild(mpBall[i] = new Sprite("image/UI/mp.png"));
		mpBall[i]->pos = Vec2(89 + 39 * i, 72);
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(goldText[i][j] = new Sprite("image/UI/dmgNum/" + to_string(j) + ".png"));
			goldText[i][j]->pos = Vec2(94 + 21 * i, 111);
			goldText[i][j]->visible = false;
		}
		goldText[i][0]->visible = true;
	}

	Sprite *timerNumber1 = new Sprite("image/UI/timerNum/0.png");
	addChild(timerNumber1);
	timerNumber1->pos = Vec2(546, 71);

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(timerText[i][j] = new Sprite("image/UI/timerNum/" + to_string(j) + ".png"));
			if (i == 0)
				timerText[i][j]->pos = Vec2(598, 71);
			else if (i == 1)
				timerText[i][j]->pos = Vec2(664, 71);
			else 
				timerText[i][j]->pos = Vec2(716, 71);

			timerText[i][j]->visible = false;
		}
		timerText[i][0]->visible = true;
	}

	for (int i = 0; i < 10; ++i)
	{
		addChild(hpPotionText[i] = new Sprite("image/UI/itemNum/" + to_string(i) + ".png"));
		hpPotionText[i]->pos = Vec2(66, 598);
		hpPotionText[i]->visible = false;
	}
	for (int i = 0; i < 10; ++i)
	{
		addChild(mpPotionText[i] = new Sprite("image/UI/itemNum/" + to_string(i) + ".png"));
		mpPotionText[i]->pos = Vec2(117, 598);
		mpPotionText[i]->visible = false;
	}

	if (stage == 1)
	{
		addChild(minimap = new Sprite("image/UI/minimap/1.png"));
	}
	if (stage == 2)
	{
		addChild(minimap = new Sprite("image/UI/minimap/2.png"));
	}
	if (stage == 3)
	{
		addChild(minimap = new Sprite("image/UI/minimap/3.png"));
	}

	minimap->pos = Vec2(1019, 21);

	addChild(resultFail = new Sprite("image/outgame/result/1.png"));
	addChild(resultClear = new Sprite("image/outgame/result/0.png"));
	resultFail->scaleCenter = resultFail->rect.center();
	resultClear->scaleCenter = resultClear->rect.center();
	resultFail->scale = Vec2(0, 0);
	resultClear->scale = Vec2(0, 0);

	addChild(black = new Sprite("image/UI/black.png"));
	black->color.a = 0;

	addChild(ed = new Animation("image/outgame/ed", 16, 10, false, true));
	ed->visible = false;
	ed->stop = true;
}

UI::~UI()
{
}
