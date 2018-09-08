#include "DXUT.h"
#include "ShopScene.h"
#include "World.h"
#include "GameManager.h"
#include "StageSelectScene.h"

ShopScene::ShopScene()
{
	Sprite *sprite = new Sprite("image/outgame/store/0.png");
	addChild(sprite);

	addChild(backB = new Sprite("image/outgame/store/undo.png"));
	backB->pos = Vec2(1105, 587);
	backB->scaleCenter = backB->rect.center();

	for (int i = 0; i < 4; i++)
	{
		addChild(button[i] = new Sprite("image/outgame/store/buyButton.png"));
		button[i]->pos = Vec2(182 + 261 * i, 503);
		button[i]->scaleCenter = button[i]->rect.center();
	}

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			addChild(goldText[i][j] = new Sprite("image/UI/dmgNum/" + to_string(j) + ".png"));
			goldText[i][j]->pos = Vec2(206 + 21 * i, 125);
			goldText[i][j]->visible = false;
		}
		goldText[i][0]->visible = true;
	}
}

ShopScene::~ShopScene()
{
}

void ShopScene::update(float dt)
{
	Scene::update(dt);

	int goldT = gameManager.gold;
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 10; ++j)
		if (goldText[2 - i][j]->visible)
			goldText[2 - i][j]->visible = false;

		goldText[2 - i][goldT % 10]->visible = true;
		goldT /= 10;
	}

	if (backB->rectWithPos().intersects(world.getMousePos()))
	{
		backB->scale = Vec2(1.2f, 1.2f);
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			world.changeScene(new StageSelectScene());
			return;
		}
	}
	else
		backB->scale = Vec2(1, 1);
	
	if (world.getKeyState('M') == 1)
	{
		gameManager.gold += 100;
	}

	if (gameManager.hpPotionCount >= 9)
		button[0]->color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1);
	if (gameManager.mpPotionCount >= 9)
		button[1]->color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1);
	if (gameManager.hpUpgradeCount >= 4)		
		button[2]->color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1);
	if (gameManager.powerUpgradeCount >=5)			
		button[3]->color = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1);

	for (int i = 0; i < 4; ++i)
	if (button[i]->rectWithPos().intersects(world.getMousePos()))
	{
		button[i]->scale = Vec2(1.2f, 1.2f);
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			if (i == 0 && gameManager.gold >= 20 && gameManager.hpPotionCount < 9)
			{
				gameManager.hpPotionCount++;
				gameManager.gold -= 20;
			}
			else if (i == 1 && gameManager.gold >= 10 && gameManager.mpPotionCount < 9)
			{
				gameManager.mpPotionCount++;
				gameManager.gold -= 10;
			}
			else if (i == 2 && gameManager.gold >= 50 && gameManager.hpUpgradeCount < 4)
			{
				gameManager.hpUpgradeCount++;
				gameManager.gold -= 50;
			}
			else if (i == 3 && gameManager.gold >= 50 && gameManager.powerUpgradeCount < 5)
			{
				gameManager.powerUpgradeCount++;
				gameManager.gold -= 50;
			}
		}
	}
	else
		button[i]->scale = Vec2(1, 1);
}