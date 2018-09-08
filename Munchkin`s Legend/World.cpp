#include "DXUT.h"
#include "World.h"
#include "LoadScene.h"
#include "StageSelectScene.h"
#include "Asset.h"

World::World(void) : currentScene(nullptr), timeScale(1)
{
}


World::~World(void)
{
}

void World::update(float dt)
{
	dt *= timeScale;

	if(currentScene) currentScene->update(dt);
	for(int i=0; i<256; i++)
	{
		lastKeys[i] = currentKeys[i];
		currentKeys[i] = GetAsyncKeyState(i) & 0x8000;
	}

	GetCursorPos(&mousePoint);
	ScreenToClient(DXUTGetHWND(), &mousePoint);

	for(auto& timer:timers)
	{
		timer->update(dt);
	}
}

void World::render()
{
	if(currentScene) currentScene->render();
}

void World::init()
{
	asset.init();
	cout.precision(3);
	D3DXCreateSprite(DXUTGetD3D9Device(), &rootSprite);
	changeScene(new LoadScene());
	ZeroMemory(lastKeys, sizeof(lastKeys));
	ZeroMemory(currentKeys, sizeof(currentKeys));
}

void World::dispose()
{
	asset.dispose();
	SAFE_RELEASE(rootSprite);
	SAFE_DELETE(currentScene);
}

void World::changeScene(Scene* scene)
{
	if(currentScene) SAFE_DELETE(currentScene);
	currentScene = scene;
}

int World::getKeyState(int vk)
{
	if(currentKeys[vk] && lastKeys[vk]) return 2;
	if(currentKeys[vk] && !lastKeys[vk]) return 1;
	if(!currentKeys[vk] && lastKeys[vk]) return -1;
	return -2;
}

Vec2 World::getMousePos()
{
	return Vec2(mousePoint.x, mousePoint.y);
}

World world;