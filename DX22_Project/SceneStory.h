// SceneStory.h
#pragma once

#include <array>
#include "Scene.h"
#include "TextureManager.h"

class UiSpriteRenderer;
class GameObject;

class SceneStory : public Scene
{
public:
    void BuildLoadTasks(std::vector<LoadTask>& outTasks) override;

    void Load() override;
    void Update() override;
    void Draw() override;
    void Unload() override;

private:
    void CreateCanvas();
    void CreatePages();

    void StartSlide(int dir);   // dir: -1=左, +1=右
    void UpdateSlide();

private:
    std::array<TextureTag, 4> m_pageTags = {
        TextureTag::Sotry_Intro1,
        TextureTag::Sotry_Intro2,
        TextureTag::Sotry_Intro3,
        TextureTag::Sotry_Intro4
    };

    int m_pageIndex = 0;

    GameObject* m_currObj = nullptr;
    UiSpriteRenderer* m_currRenderer = nullptr;

    GameObject* m_nextObj = nullptr;
    UiSpriteRenderer* m_nextRenderer = nullptr;

    // 入力取りこぼし防止（スライド中に押された入力を予約）
    int m_pendingDir = 0;

    // スライド状態
    bool  m_isSliding = false;
    int   m_slideDir = 0;
    float m_slideT = 0.0f;
    float m_slideDuration = 0.35f;   // スライド時間

    // UI設定
    Vector2 m_center2D = Vector2(0.0f, 0.0f);          // 中心（あなたのUI基準）
    float   m_slideOffsetPx = 1600.0f;                 // 画面外へ出す距離（px）
    Vector2 m_uiSize = Vector2(1500.0f, 900.0f);       // 画像サイズ
};