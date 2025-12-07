// SceneSmaple.h
#pragma once
#include "Scene.h"

class SceneSmaple : public Scene
{
public:
    SceneSmaple() = default;
    ~SceneSmaple() override = default;

    void Load() override;
    void Update() override;
    void Draw() override;
    void Unload() override;
};
