// SceneTitle.h
#pragma once
#include "Scene.h"

class SceneTitle : public Scene
{
public:
    SceneTitle() = default;
    ~SceneTitle() override = default;

    void Load() override;
    void Update() override;
    void Draw() override;
    void Unload() override;
};
