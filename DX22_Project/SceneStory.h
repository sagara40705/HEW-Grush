// SceneStory.h
#pragma once

#include <array>
#include <vector>
#include <functional>

#include "Scene.h"
#include "TextureManager.h"

class UiSpriteRenderer;
class GameObject;

class SceneStory : public Scene
{
public:
    // Scene ‚ª—v‹‚µ‚Ä‚¢‚éƒˆ‰¼‘zŠÖ”‚ğ•K‚¸À‘•‚·‚é
    void Load() override;
    void Draw() override;
    void Unload() override;

    // Šù‚Éì‚Á‚Ä‚¢‚é‚È‚ç‚»‚Ì‚Ü‚Ü‚ÅOK
    void BuildLoadTasks(std::vector<std::function<void()>>& outTasks) override;
    void Update() override;

private:
    GameObject* m_introObj = nullptr;
    UiSpriteRenderer* m_introRenderer = nullptr;

    std::array<TextureTag, 4> m_introTags = {
        TextureTag::Sotry_Intro1,
        TextureTag::Sotry_Intro2,
        TextureTag::Sotry_Intro3,
        TextureTag::Sotry_Intro4
    };

    int   m_index = 0;
    float m_timer = 0.0f;
    float m_interval = 1.2f;

private:
    void CreateIntro();
    void NextOrFinish();
};