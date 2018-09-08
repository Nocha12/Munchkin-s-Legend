#pragma once
#include "Scene.h"
#include "Sprite.h"

class StageSelectScene : public Scene
{
public:
	StageSelectScene();
	~StageSelectScene();

	void update(float dt);

	Sprite *frame;
	Sprite *lock[2];
	Sprite *storeB;
	Sprite *backB;
};

