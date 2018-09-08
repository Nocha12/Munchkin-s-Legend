#include "DXUT.h"
#include "StageSelectScene.h"
#include "GameManager.h"
#include "World.h"
#include "GameScene.h"
#include "MainScene.h"
#include "ShopScene.h"

StageSelectScene::StageSelectScene()
{
	Sprite *sprite = new Sprite("image/outgame/select/0.png");
	addChild(sprite);
	addChild(frame = new Sprite("image/outgame/select/frame.png"));
	addChild(lock[0] = new Sprite("image/outgame/select/lock.png"));
	addChild(lock[1] = new Sprite("image/outgame/select/lock.png"));
	addChild(storeB = new Sprite("image/outgame/select/store.png"));
	addChild(backB = new Sprite("image/outgame/select/undo.png"));
	
	frame->pos = Vec2(156, 183);
	storeB->pos = Vec2(127, 572);
	backB->pos = Vec2(1104, 587);
	lock[0]->pos = Vec2(518, 218);
	lock[1]->pos = Vec2(848, 218);

	backB->scaleCenter = backB->rect.center();
	storeB->scaleCenter = storeB->rect.center();

	if (gameManager.stage > 1)
		lock[0]->visible = false;
	if (gameManager.stage > 2)
		lock[1]->visible = false;
}

StageSelectScene::~StageSelectScene()
{
}

void StageSelectScene::update(float dt)
{
	Scene::update(dt);

	if (world.getKeyState(VK_RETURN) == 1)
	{
		if (frame->pos == Vec2(156, 183))
		{
			world.changeScene(new GameScene(1));
			return;
		}
		else if (frame->pos == Vec2(486, 183) && gameManager.stage > 1)
		{
			world.changeScene(new GameScene(2));
			return;
		}
		else if (frame->pos == Vec2(816, 183) && gameManager.stage > 2)
		{
			world.changeScene(new GameScene(3));
			return;
		}
	}

	if (backB->rectWithPos().intersects(world.getMousePos()))
	{
		backB->scale = Vec2(1.2f, 1.2f);
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			world.changeScene(new MainScene());
			return;
		}
	}
	else
		backB->scale = Vec2(1, 1);

	if (storeB->rectWithPos().intersects(world.getMousePos()))
	{
		storeB->scale = Vec2(1.2f, 1.2f);
		if (world.getKeyState(VK_LBUTTON) == 1)
		{
			world.changeScene(new ShopScene());
			return;
		}
	}
	else
		storeB->scale = Vec2(1, 1);

	if (world.getKeyState(VK_LEFT) == 1)
		frame->pos.x = frame->pos.x - 330 < 156 ? 156 : frame->pos.x - 330;
	if (world.getKeyState(VK_RIGHT) == 1)
		frame->pos.x = frame->pos.x + 330 > 816 ? 816 : frame->pos.x + 330;

	if (world.getKeyState('O') == 1)
		gameManager.stage = 3;
}