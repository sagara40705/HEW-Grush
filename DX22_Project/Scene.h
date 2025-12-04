#ifndef __SCENE_H__
#define __SCENE_H__

#include "World.h"

class Scene
{
public:
	Scene();
	virtual ~Scene();
public:
	void RootLoad();
	void RootUpdate();
	void RootDraw();
	void RootUnload();
public:
	virtual void Load() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void Unload() = 0;
public:
	// この Scene 専用の World へのアクセス
	World& GetWorld() { return world; }
	const World& GetWorld() const { return world; }
private:
	World world;
	bool onLoad = false;
	bool onUnload = false;
};



#endif // __SCENE_H__