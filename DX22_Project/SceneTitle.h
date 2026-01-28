 /*SceneTitle.h
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
};*/
// SceneTitle.h
#pragma once
#include "Scene.h"
#include "Component.h"
//#include "BreakUiRenderer.h"

class SceneTitle : public Scene
{
public:
    SceneTitle() = default;
    ~SceneTitle() override = default;

    // 非同期ロード用
    void BuildLoadTasks(std::vector<LoadTask>& outTasks) override;

    void Load() override;
    void Update() override;
    void Draw() override;
    void Unload() override;
private:
    void CreateCameraAndPlayer();
    void CreateCanvas();
    void CreateBackGround();
    void CreateTitleFont();
    void CreateTitleLogo();
    void CreateBreakTexture();
    void CreateTitleManager();
    void UpdateTitleAnimations(); 
private:
    float m_animTimer = 0.0f;

    // 生成したオブジェクトを動かすためのポインタ
    GameObject* m_titleLogoObj = nullptr;
    GameObject* m_titleFontObj = nullptr;
    GameObject* m_mainCamera = nullptr;
    GameObject* m_player = nullptr;
    SpriteAnimation* break_ = nullptr;


    bool m_isStarting = false;    // 演出中フラグ
    float m_zoomTimer = 0.0f;     // 経過時間
    const float ZOOM_DURATION = 0.5f; // ズームにかける時間（秒）

    Vector3 m_camStartPos;        // 移動開始時のカメラ座標
    Vector3 m_camEndPos = Vector3(0.f, 0.5f, 1.f); // 最終的なカメラ座標（プレイヤーの目の前）
};

