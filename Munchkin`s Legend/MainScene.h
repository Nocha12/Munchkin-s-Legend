#pragma once
#include "Scene.h"
#include "Animation.h"

class MainScene :
	public Scene
{
public:
	MainScene();
	~MainScene();
	
	Sprite *gameStartB;
	Sprite *exitB;
	Sprite *howToB;
	Sprite *introB;
	Sprite *howToEx[2];
	Sprite *introEx;

	int howToCount;

	void update(float dt);
};

