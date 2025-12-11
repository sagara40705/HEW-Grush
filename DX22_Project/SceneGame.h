#ifndef __SCENE_GAME_H__
#define __SCENE_GAME_H__

#include "Scene.h"
#include "CSV.h"

class SceneGame : public Scene
{
public:
	SceneGame();
	~SceneGame();
	void Load() final;
	void Update() final;
	void Draw() final;
	void Unload() final;

private:
	void RegisterTexture();
	void RegisterModel();
	void CreateProtStage();
	void CreateStageFromCSV(const std::string& filename);
	void CreateGlassModel();
	void SetPlayerAndHummer();
	void CreateTimerUi();
	void CreateScoreUi();
	void CreateActionUi();
};

#endif // __SCENE_GAME_H__