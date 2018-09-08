#include "DXUT.h"
#include "MainScene.h"
#include "World.h"
#include "StageSelectScene.h"

MainScene::MainScene() : howToCount(-1)
{
	Animation *ani = new Animation("image/outgame/title/ani", 7, 8, true, true);
	addChild(ani);

	Sprite *title = new Sprite("image/outgame/title/0.png");
	addChild(title);
	addChild(gameStartB = new Sprite("image/outgame/title/gameStart.png"));
	addChild(introB = new Sprite("image/outgame/title/introB.png"));
	addChild(howToB = new Sprite("image/outgame/title/howto.png"));
	addChild(exitB = new Sprite("image/outgame/title/exit.png"));
	addChild(howToEx[0] = new Sprite("image/outgame/title/how0.png"));
	addChild(howToEx[1] = new Sprite("image/outgame/title/how1.png"));
	addChild(introEx = new Sprite("image/outgame/title/intro.png"));
	howToEx[0]->visible = false;
	howToEx[1]->visible = false;
	introEx->visible = false;

	gameStartB->scaleCenter = gameStartB->rect.center();
	introB->scaleCenter = introB->rect.center();
	howToB->scaleCenter = howToB->rect.center();
	exitB->scaleCenter = exitB->rect.center();

	title->pos = Vec2(606, 71);
	gameStartB->pos = Vec2(938, 361);
	introB->pos = Vec2(971, 512);
	howToB->pos = Vec2(910, 437);
	exitB->pos = Vec2(972, 586);
}

MainScene::~MainScene()
{
}

void MainScene::update(float dt)
{
	Scene::update(dt);

	if (howToCount >= 0)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			if (howToCount < 1)
			{
				howToEx[howToCount]->visible = false;
				howToEx[++howToCount]->visible = true;
			}
			else
			{
				howToEx[1]->visible = false;
				howToCount = -1;
			}
		}
	}
	else if (introEx->visible)
	{
		if (world.getKeyState(VK_LBUTTON) == 1)
			introEx->visible = false;
	}
	else
	{
		if (gameStartB->rectWithPos().intersects(world.getMousePos()))
		{
			gameStartB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				world.changeScene(new StageSelectScene());
				return;
			}
		}
		else
			gameStartB->scale = Vec2(1, 1);

		if (howToB->rectWithPos().intersects(world.getMousePos()))
		{
			howToB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				howToCount = 0;
				howToEx[howToCount]->visible = true;
			}
		}
		else
			howToB->scale = Vec2(1, 1);

		if (introB->rectWithPos().intersects(world.getMousePos()))
		{
			introB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				introEx->visible = true;
			}
		}
		else
			introB->scale = Vec2(1, 1);

		if (exitB->rectWithPos().intersects(world.getMousePos()))
		{
			exitB->scale = Vec2(1.2f, 1.2f);
			if (world.getKeyState(VK_LBUTTON) == 1)
			{
				exit(1);
				return;
			}
		}
		else
			exitB->scale = Vec2(1, 1);
	}
}

