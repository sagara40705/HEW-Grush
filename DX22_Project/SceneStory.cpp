// SceneStory.cpp
#include "SceneStory.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "InputKeyboard.h"
#include "UiSpriteRenderer.h"
#include "SceneStageSelect.h"

void SceneStory::BuildLoadTasks(std::vector<LoadTask>& outTasks)
{
    outTasks.emplace_back([this] { GetWorld().ClearAll(); });

    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro1); });
    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro2); });
    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro3); });
    outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Sotry_Intro4); });

    outTasks.emplace_back([this] { CreateCanvas(); });
    outTasks.emplace_back([this] { CreatePages(); });

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
    TextureManager::Clear();
}

void SceneStory::Draw()
{
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

    // 現在ページ（中心）
    {
        GameObject& obj = w.CreateGameObject(m_centerPos);

        UiSpriteRenderer& r = obj.AddComponent<UiSpriteRenderer>(
            m_pageTags[0],
            Vector2(0.0f, 0.0f),
            Vector2(1500.0f, 900.0f)
        );

        m_currObj = &obj;
        m_currRenderer = &r;
    }

    // 次ページ（右の画面外）
    {
        Vector3 offPos(m_centerPos.x + m_slideOffsetX, m_centerPos.y, m_centerPos.z);
        GameObject& obj = w.CreateGameObject(offPos);

        UiSpriteRenderer& r = 
            obj.AddComponent<UiSpriteRenderer>(
            m_pageTags[0],
            Vector2(0.0f, 0.0f),
            Vector2(1500.0f, 900.0f)
        );

        m_nextObj = &obj;
        m_nextRenderer = &r;
    }

    m_pageIndex = 0;
    m_isSliding = false;
    m_slideDir = 0;
    m_slideT = 0.0f;
}

void SceneStory::Update()
{
    if (m_isSliding)
    {
        UpdateSlide();
        return;
    }

    // 右
    if (InputKeyboard::GetKeyDown(VK_RIGHT))
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
    if (InputKeyboard::GetKeyDown(VK_LEFT))
    {
        if (m_pageIndex <= 0)
        {
            return;
        }

        StartSlide(-1);
        return;
    }
}

void SceneStory::StartSlide(int dir)
{
    if (!m_currObj || !m_nextObj || !m_currRenderer
        || !m_nextRenderer) return;

    int nextIndex = m_pageIndex + dir;
    if (nextIndex < 0 || nextIndex >= (int)m_pageTags.size()) return;

    m_isSliding = true;
    m_slideDir = dir;
    m_slideT = 0.0f;

    m_nextRenderer->SetTexture(m_pageTags[nextIndex]);

    float startX = m_centerPos.x + (dir > 0 ? +m_slideOffsetX : -m_slideOffsetX);

    Transform* currTr = m_currObj->GetComponent<Transform>();
    Transform* nextTr = m_nextObj->GetComponent<Transform>();

    if (currTr) currTr->SetPosition(m_centerPos);
    if (nextTr) nextTr->SetPosition(Vector3(startX, m_centerPos.y, m_centerPos.z));
}

void SceneStory::UpdateSlide()
{
    const float DT = 1.0f / 60.0f;

    m_slideT += DT;

    float t = (m_slideDuration > 0.0f) ? (m_slideT / m_slideDuration) : 1.0f;
    if (t > 1.0f) t = 1.0f;

    float ease = t * t * (3.0f - 2.0f * t);

    Transform* currTr = m_currObj ? m_currObj->GetComponent<Transform>() : nullptr;
    Transform* nextTr = m_nextObj ? m_nextObj->GetComponent<Transform>() : nullptr;

    float currX = m_centerPos.x + (-m_slideDir) * m_slideOffsetX * ease;

    float nextStartX = m_centerPos.x + (m_slideDir > 0 ? +m_slideOffsetX : -m_slideOffsetX);
    float nextX = nextStartX + (m_centerPos.x - nextStartX) * ease;

    if (currTr) currTr->SetPosition(Vector3(currX, m_centerPos.y, m_centerPos.z));
    if (nextTr) nextTr->SetPosition(Vector3(nextX, m_centerPos.y, m_centerPos.z));

    if (t >= 1.0f)
    {
        m_pageIndex += m_slideDir;

        std::swap(m_currObj, m_nextObj);
        std::swap(m_currRenderer, m_nextRenderer);

        Transform* tr = m_currObj ? m_currObj->GetComponent<Transform>() : nullptr;
        if (tr) tr->SetPosition(m_centerPos);

        m_isSliding = false;
        m_slideDir = 0;
        m_slideT = 0.0f;
    }
}