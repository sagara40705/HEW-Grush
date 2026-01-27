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

    void StartSlide(int dir);
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

    bool  m_isSliding = false;
    int   m_slideDir = 0;
    float m_slideT = 0.0f;
    float m_slideDuration = 0.25f;

    // あなたの環境でUiSpriteRendererが表示できた中心位置
    Vector3 m_centerPos = Vector3(0.f, -3.f, 5.f);

    // 画面外に退避させる距離
    float m_slideOffsetX = 6.0f;
};