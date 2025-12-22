#pragma once
#include "Scene.h"

class SceneStageSelect : public Scene
{
public:
    SceneStageSelect() = default;
    ~SceneStageSelect() override = default;

    void Load() override;
    void Update() override;
    void Draw() override;
    void Unload() override;
};


