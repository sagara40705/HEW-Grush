#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();
	void Update() final;
	void Draw() final;

private:
	void SetPlayerAndHummer();
	void CreateTimerUi();
};

#endif // __SCENE_GAME_H__