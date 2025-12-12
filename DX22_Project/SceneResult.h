#pragma once
#include "Scene.h"
class Result : public Scene
{
public:
    Result() = default;
    ~Result() override = default;

    void Load() override;
    void Update() override;
    void Draw() override;
    void Unload() override;
};


