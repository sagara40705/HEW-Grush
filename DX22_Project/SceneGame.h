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
	void RegisterTexture();
	void CreateGlassModel();
	void SetPlayerAndHummer();
	void CreateTimerUi();
	void CreateScoreUi();
	void CreateActionUi();
};

#endif // __SCENE_GAME_H__