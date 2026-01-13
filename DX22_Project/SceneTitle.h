#pragma once

// SceneTitle.h
#pragma once
#include "Scene.h"
#include"BreakUiRenderer.h"

class SceneTitle : public Scene
{
public:
    SceneTitle() = default;
    ~SceneTitle() override = default;

    void Load() override;
    void Update() override;
    void Draw() override;
    void Unload() override;
  
private:
    // ★ エンターで割る対象のボタン
    BreakUiRenderer* m_breakButton = nullptr;
};
