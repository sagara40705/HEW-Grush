#pragma once
#include "Scene.h"

class SceneStageSelect : public Scene
{
public:
    SceneStageSelect() = default;
    ~SceneStageSelect() override = default;

    void Load() override;
    void Unload() override;
   
    void BuildLoadTasks(std::vector<LoadTask>& outTasks) override;
private:
	void CreateCanvas();
	void CreateStageMap();
    void CreateCamera();
    void CreatePlayer();
    void CreateGround();
	void CreateStageLevelUI();
	void CreateStageImageUI();
public:
    void Update() override;
    void Draw() override;
private:

};


