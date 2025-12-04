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
	// ”ñ“¯Šúƒ[ƒh—p
	void BuildLoadTasks(std::vector<LoadTask>& outTasks) override;
private:
	void LoadModels();
	void LoadTextures();
	void LoadSound() {}

	void CreateManagers();
	void CreateCameraAndPlayer();
	void CreateStage();
	void CreateUi();
	void CreateScoreUI();
};

#endif // __SCENE_GAME_H__