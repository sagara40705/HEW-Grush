#include "OptionUIController.h"
#include "InputKeyboard.h"
#include "GameObject.h"
#include "World.h"

void OptionUIController::Start()
{
    // 最初は閉じておく（CreateOptionUI側で SetActive(false) してもOK）
    m_isOpen = gameObject->IsActiveInHierarchy();
}

void OptionUIController::Update()
{
    // Esc で開閉
    if (InputKeyboard::GetKeyDown(VK_ESCAPE))
    {
        if (!m_isOpen) Open();
        else           Close();
    }
}

void OptionUIController::Open()
{
    m_isOpen = true;

    // Root を表示
    gameObject->SetActive(true);

    // ゲームを止めたい場合：
    // UI階層以外の MonoBehaviour を無効化する（描画(Renderer)は無効化しないので画面は残る）
    World* w = gameObject->world;
    if (!w) return;

    m_paused.clear();

    for (auto& objPtr : w->objects)
    {
        GameObject* go = objPtr.get();
        if (!go) continue;

        // オプションUI（自分のRoot配下）は止めない
        if (IsInThisHierarchy(go)) continue;

        // GameObject に付いてる Component を見て、MonoBehaviour だけ止める
        for (auto& kv : go->components)
        {
            Component* c = kv.second.get();
            if (!c) continue;

            MonoBehaviour* mb = dynamic_cast<MonoBehaviour*>(c);
            if (!mb) continue;

            // 既に無効なものは触らない
            if (!mb->IsEnabled()) continue;

            mb->SetEnabled(false);
            m_paused.push_back(mb);
        }
    }
}

void OptionUIController::Close()
{
    m_isOpen = false;

    // 止めたコンポーネントを戻す
    for (Component* c : m_paused)
    {
        if (!c) continue;
        c->SetEnabled(true);
    }
    m_paused.clear();

    // Root を非表示
    gameObject->SetActive(false);
}

bool OptionUIController::IsInThisHierarchy(GameObject* go) const
{
    // go が「この OptionUIController の gameObject（Root）」配下か調べる
    if (!go) return false;

    GameObject* cur = go;
    while (cur)
    {
        if (cur == gameObject) return true;
        cur = cur->GetParent();
    }
    return false;
}