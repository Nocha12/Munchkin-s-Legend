#pragma once
#include "Scene.h"
#include "Sprite.h"

class ShopScene : public Scene
{
public:
	ShopScene();
	~ShopScene();

	Sprite *button[4];
	Sprite *backB;
	Sprite *goldText[3][10];

	void update(float dt);
};

