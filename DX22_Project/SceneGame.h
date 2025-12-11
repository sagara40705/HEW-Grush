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
	void Load() final;
	void Unload() final;
	// îÒìØä˙ÉçÅ[Éhóp
	void BuildLoadTasks(std::vector<LoadTask>& outTasks) override;
private:
	void LoadModels();
	void LoadTextures();
	void LoadSound() {}

	void CreateManagers();
	void CreateCameraAndPlayer();
	void CreateStage();
	void CreateUi();
	void CreateGaugeUI();
	void CreateTimerUI();
	void CreateScoreUI();
	void CreateActionUI();

	void InitPlayerTex();
};

#endif // __SCENE_GAME_H__