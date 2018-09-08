#include "DXUT.h"
#include "MultiDirAnimation.h"

MultiDirAnimation::MultiDirAnimation(Unit *target, string path, int fileNum, float fps, bool isLoop, bool isPng) : target(target), isPlaying(true)
{
	for (int i = 0; i < 8; ++i)
	{
		animation[i] = new Animation(path + "/" + to_string(i), fileNum, fps, isLoop, isPng);
		addChild(animation[i]);
	}
	rect = animation[0]->rect;
	direction = 0;
	size = animation[0]->textures.size() - 1;
}

MultiDirAnimation::~MultiDirAnimation()
{
}

void MultiDirAnimation::update(float dt)
{
	Entity::update(dt);
	
	int angle = (int)D3DXToDegree(target->moveRotation) - 180 - 22;

	if (angle < 0) angle += 360;

	direction = angle / 45;
	animation[direction]->update(dt);
	animation[direction]->visible = true;
	currentFrame = animation[direction]->currentFrame;
	for (int i = 0; i < 8; ++i)
		if (i != direction)
			animation[i]->visible = false;

	if (currentFrame >= animation[direction]->textures.size() - 1)
		isPlaying = false;
}

void MultiDirAnimation::render()
{
	Entity::render();
	if (visible)
		animation[direction]->render();
}

void MultiDirAnimation::Reset()
{
	for (int i = 0; i < 8; ++i)
	{
		animation[i]->currentFrame = 0;
	}
}

void MultiDirAnimation::Stop()
{
	for (int i = 0; i < 8; ++i)
	{
		animation[i]->stop = true;
	}
}

void MultiDirAnimation::Play()
{
	isPlaying = true;

	for (int i = 0; i < 8; ++i)
	{
		animation[i]->stop = false;
	}
}

void MultiDirAnimation::Invisible()
{
	for (int i = 0; i < 8; ++i)
		animation[i]->color = D3DXCOLOR(1, 1, 1, 0);
}

void MultiDirAnimation::FaidIn()
{
	faidTimer.reset(0.01f, 100);
	faid = 0;

	faidTimer.onTick = [=]()
	{
		faid += 0.01f;

		for (int i = 0; i < 8; ++i)
			animation[i]->color = D3DXCOLOR(1, 1, 1, faid);
	};
}

void MultiDirAnimation::FaidOut()
{
	faidTimer.reset(0.01f, 100);
	faid = 1;

	faidTimer.onTick = [=]()
	{
		faid -= 0.01f;

		for (int i = 0; i < 8; ++i)
			animation[i]->color = D3DXCOLOR(1, 1, 1, faid);
	};
}

void MultiDirAnimation::GetRed()
{
	for (int i = 0; i < 8; ++i)
	{
		animation[i]->color.b = 0.3f;
		animation[i]->color.g = 0.3f;
	}
	redTimer.reset(0.01f, 25);
	redTimer.onTick = [=](){
		for (int i = 0; i < 8; ++i)
		{
			animation[i]->color.b += 0.028f;
			animation[i]->color.g += 0.028f;
		}
	};
}

void MultiDirAnimation::GetBlack()
{
	for (int i = 0; i < 8; ++i)
	{
		animation[i]->color.r = 0.3f;
		animation[i]->color.b = 0.3f;
		animation[i]->color.g = 0.3f;
	}
	blackTimer.reset(0.01f, 25);
	blackTimer.onTick = [=](){
		for (int i = 0; i < 8; ++i)
		{
			animation[i]->color.r += 0.028f;
			animation[i]->color.b += 0.028f;
			animation[i]->color.g += 0.028f;
		}
	};
}
