#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Unit.h"
#include "Timer.h"

class MultiDirAnimation : public Entity
{
public:
	MultiDirAnimation(Unit *target, string path, int fileNum, float fps = 10, bool isLoop = true, bool isPng = false);
	~MultiDirAnimation();

	void update(float dt);
	void render();

	void Reset();
	void Stop();
	void Play();
	void FaidIn();
	void FaidOut();
	void Invisible();
	void GetRed();
	void GetBlack();

	Unit *target;
	Animation *animation[8];

	Timer faidTimer;
	Timer redTimer;
	Timer blackTimer;

	float currentFrame;
	int direction;
	int size;
	float faid;
	bool isPlaying;
};

