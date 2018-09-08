#include "DXUT.h"
#include "DropBox.h"
#include "World.h"
#include "GameScene.h"
 
DropBox::DropBox(int stage) : stage(stage), isFaidIn(true), isDropItem(false)
{	
	addChild(ani = new Animation("image/box", 21, 10, false, true));
	rect = ani->rect;
	ani->stop = true;
	ani->color.a = 0;
}

DropBox::~DropBox()
{
}

void DropBox::update(float dt)
{
	Entity::update(dt);
	GameScene *scene = (GameScene*)world.currentScene;

	if (isFaidIn && !ani->stop && ani->color.a < 1)
		ani->color.a += 0.02f;
	if (ani->currentFrame >= ani->textures.size() - 1 && ani->color.a > 1)
	{
		if (!isDropItem)
		{
			isDropItem = true;
			makeItemTimer.reset(0.08f, 20);
			makeItemTimer.onTick = [=](){
				Item *item = nullptr;

				if (random(0, 20) == 0)
				{
					if (random(0, 1) == 0)
						item = new Item(HpPotion);
					else
						item = new Item(MpPotion);
				}
				else
					item = new Item(GoldItem);
				scene->addChild(item);
				scene->itemList.push_back(item);
				item->setCenter(center());
				item->speed += random(10, 100);
			};
			makeItemTimer.onFinished = [=](){
				if (stage != 3)
				{
					Item *item = nullptr;

					if (stage == 1)
						item = new Item(Necklace);
					else if (stage == 2)
						item = new Item(Ring);

					scene->addChild(item);
					scene->itemList.push_back(item);
					item->setCenter(center());
				}
			};
		}
		isFaidIn = false;
		ani->color.a -= 0.005f;
	}
}