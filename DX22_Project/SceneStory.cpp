// SceneStory.cpp
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "InputKeyboard.h"
#include "UiSpriteRenderer.h"
#include "SceneStageSelect.h"
#include"SceneStory.h"
#include"InputGamepad.h"

void SceneStory::BuildLoadTasks(std::vector<LoadTask>& outTasks)
{
    outTasks.emplace_back([this] { GetWorld().ClearAll(); });

    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro1); });
    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro2); });
    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro3); });
    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro4); });

    outTasks.emplace_back([this] { CreateCanvas(); });
    outTasks.emplace_back([this] { CreatePages(); });

    // 追加：フェード用アニメーションをこのSceneのWorldに作る
    outTasks.emplace_back([this] { SceneManager::CreateFadeAnimations(this); });

    outTasks.emplace_back([this] { GetWorld().Start(); });
}

void SceneStory::Load()
{
    std::vector<LoadTask> tasks;
    BuildLoadTasks(tasks);
    for (auto& t : tasks) t();
}

void SceneStory::Unload()
{
    SceneManager::ReleaseFadeAnimations();
    TextureManager::Clear();
}

void SceneStory::Draw()
{
    // 他Sceneと同じで空
}

void SceneStory::CreateCanvas()
{
    World& w = GetWorld();
    GameObject& canvas = w.CreateGameObject();
    canvas.AddComponent<Canvas>();
}

void SceneStory::CreatePages()
{
    World& w = GetWorld();

    // 注意：
    // UiSpriteRendererは「offset(2D)」で位置を決めるので、
    // GameObjectのTransformは固定でOK（描画される座標に置く）
    Vector3 uiObjPos(0.f, -3.f, 5.0f);

    // 現在ページ（中心）
    {
        GameObject& obj = w.CreateGameObject(uiObjPos);
        
        UiSpriteRenderer& r = obj.AddComponent<UiSpriteRenderer>(
            m_pageTags[0],
            m_center2D,      // offset = 中心
            m_uiSize
        );
        m_currObj = &obj;
        m_currRenderer = &r;
    }

    // 次ページ（最初は右の画面外）
    {
        GameObject& obj = w.CreateGameObject(uiObjPos);
        UiSpriteRenderer& r = obj.AddComponent<UiSpriteRenderer>(
            m_pageTags[0],
            Vector2(m_center2D.x + m_slideOffsetPx, m_center2D.y),
            m_uiSize
        );
        m_nextObj = &obj;
        m_nextRenderer = &r;
    }

    m_pageIndex = 0;
    m_isSliding = false;
    m_slideDir = 0;
    m_slideT = 0.0f;
    m_pendingDir = 0;
}

void SceneStory::Update()
{
    // スライド中は入力を予約しつつスライド更新
    if (m_isSliding)
    {
        if (InputKeyboard::GetKeyDown(VK_RIGHT)
            ||InputGamepad::GetButtonDown(XINPUT_GAMEPAD_X)) m_pendingDir = +1;
        if (InputKeyboard::GetKeyDown(VK_LEFT)
            || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_Y))  m_pendingDir = -1;

        UpdateSlide();
        return;
    }

    // 右
    if (InputKeyboard::GetKeyDown(VK_RIGHT)
        || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_X))
    {
        // 4枚目でさらに右ならステージセレクトへ
        if (m_pageIndex >= 3)
        {
            SceneManager::ChangeSceneAsync<SceneStageSelect>();
            return;
        }

        StartSlide(+1);
        return;
    }

    // 左
    if (InputKeyboard::GetKeyDown(VK_LEFT)
        || InputGamepad::GetButtonDown(XINPUT_GAMEPAD_Y))
    {
        if (m_pageIndex <= 0) return;

        StartSlide(-1);
        return;
    }
}

void SceneStory::StartSlide(int dir)
{
    if (!m_currRenderer || !m_nextRenderer) return;

    int nextIndex = m_pageIndex + dir;
    if (nextIndex < 0 || nextIndex >= (int)m_pageTags.size()) return;

    m_isSliding = true;
    m_slideDir = dir;
    m_slideT = 0.0f;

    // 次ページのテクスチャを設定
    m_nextRenderer->SetTexture(m_pageTags[nextIndex]);

    // 現在は中心
    m_currRenderer->SetOffset(m_center2D);

    // 次は左右の画面外から入る
    float startX = m_center2D.x + (dir > 0 ? +m_slideOffsetPx : -m_slideOffsetPx);
    m_nextRenderer->SetOffset(Vector2(startX, m_center2D.y));
}

void SceneStory::UpdateSlide()
{
    const float DT = 1.0f / 60.0f;

    m_slideT += DT;

    float t = (m_slideDuration > 0.0f) ? (m_slideT / m_slideDuration) : 1.0f;
    if (t > 1.0f) t = 1.0f;

    // ease in-out
    float ease = t * t * (3.0f - 2.0f * t);

    // 現在：中心 -> 反対側へ出る
    float currX = m_center2D.x + (-m_slideDir) * m_slideOffsetPx * ease;

    // 次：画面外 -> 中心へ入る
    float nextStartX = m_center2D.x + (m_slideDir > 0 ? +m_slideOffsetPx : -m_slideOffsetPx);
    float nextX = nextStartX + (m_center2D.x - nextStartX) * ease;

    m_currRenderer->SetOffset(Vector2(currX, m_center2D.y));
    m_nextRenderer->SetOffset(Vector2(nextX, m_center2D.y));

    if (t >= 1.0f)
    {
        // ページ確定
        m_pageIndex += m_slideDir;

        // 次が現在になるので入れ替え
        std::swap(m_currObj, m_nextObj);
        std::swap(m_currRenderer, m_nextRenderer);

        // 新しい現在は中心に戻す
        m_currRenderer->SetOffset(m_center2D);

        // スライド終了
        m_isSliding = false;
        m_slideDir = 0;
        m_slideT = 0.0f;

        // 予約入力があれば続けて実行
        if (m_pendingDir != 0)
        {
            int dir = m_pendingDir;
            m_pendingDir = 0;

            if (dir > 0)
            {
                if (m_pageIndex >= 3)
                {
                    SceneManager::ChangeSceneAsync<SceneStageSelect>();
                    return;
                }
                StartSlide(+1);
                return;
            }
            else
            {
                if (m_pageIndex <= 0) return;
                StartSlide(-1);
                return;
            }
        }
    }
}