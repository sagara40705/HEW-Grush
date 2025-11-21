#include "Scene.h"

Scene::Scene()
{
}
Scene::~Scene()
{
}

void Scene::RootUpdate()
{
	world.Update();
	Update();
}
void Scene::RootDraw()
{
	world.Draw();
	Draw();
}

void Scene::RootLoad()
{
	if (onLoad) return;

	onLoad = true;
	world.ClearAll();
	Load();

	world.Start();
}

void Scene::RootUnload()
{
	if (onUnload) return;

	onUnload = true;
	Unload();

	world.ClearAll();
}
