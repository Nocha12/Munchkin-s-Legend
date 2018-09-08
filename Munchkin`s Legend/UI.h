#pragma once
#include "Entity.h"
#include "Animation.h"

class UI : public Entity
{
public:
	UI(int stage);
	~UI();

	Sprite *stageText;
	Sprite *minimap;
	Sprite *hpBar;
	Sprite *mpBall[6];
	Sprite *hpPotionText[10];
	Sprite *mpPotionText[10];
	Sprite *goldText[3][10];
	Sprite *timerText[3][10];
	Sprite *resultFail;
	Sprite *resultClear;
	Sprite *skillD;
	Sprite *emptyMpBall;
	Animation *ed;
	Sprite *black;
};

