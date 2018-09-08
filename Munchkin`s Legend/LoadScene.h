#pragma once
#include "Scene.h"
#include "Animation.h"

class LoadScene :
	public Scene
{
public:
	LoadScene();
	~LoadScene();

	void update(float dt);

	Sprite *bar;
	Animation *text;
	Animation *op;
};

