#include "World.h"
#include "GameObject.h"
#include "Component.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "DebugLog.h"

#include <algorithm>

//====================== ctor / dtor ======================

World::World() = default;
World::~World() = default;

//====================== 生成・破棄・検索 ======================

void World::RequestStart(GameObject* go)
{
    if (!go) return;
    pendingStartObjects.push_back(go);
}

GameObject& World::CreateGameObject()
{
    auto obj = std::make_unique<GameObject>();
    obj->id = nextId++;
    obj->world = this;

    GameObject* raw = obj.get();
    objects.emplace_back(std::move(obj));

    // ★ 新規生成も Start 対象
    raw->pendingStart = true;
    pendingStartObjects.push_back(raw);

    return *objects.back();
}

void World::RequestDestroy(ObjectId id)
{
    if (std::find(toDestroy.begin(), toDestroy.end(), id) == toDestroy.end())
    {
        toDestroy.push_back(id);
    }
}

GameObject* World::Find(ObjectId id) noexcept
{
    for (auto& up : objects)
    {
        if (up && up->id == id) return up.get();
    }
    return nullptr;
}

const GameObject* World::Find(ObjectId id) const noexcept
{
    for (auto& up : objects)
    {
        if (up && up->id == id) return up.get();
    }
    return nullptr;
}

//====================== ライフサイクル ======================

void World::Start()
{
    Debug::Log("World::Start()");

    // すべてのコンポーネントに対して Start 呼び出し
    for (auto& up : objects)
    {
        if (!up) continue;
        auto& o = *up;
        for (auto& kv : o.components)
        {
            if (auto* c = kv.second.get())
            {
                c->RootStart();
            }
        }
    }
}

void World::Update()
{
    constexpr float fixedDt = 1.0f / 60.0f;

    auto isPending = [this](ObjectId id)
        {
            return std::find(toDestroy.begin(), toDestroy.end(), id) != toDestroy.end();
        };

    //-1) GameObject

    for (auto& up : objects)
    {
        if (!up || !up->isDestroyScheduled) continue;

        up->DestroyTimerUpdate();
    }

    // 0) Start 待ち GameObject の Start を実行
    if (!pendingStartObjects.empty())
    {
        for (auto* go : pendingStartObjects)
        {
            if (!go) continue;
            if (!go->active) continue;
            if (isPending(go->id)) continue;

            // 全コンポーネントに RootStart（中で onStart チェック）
            for (auto& kv : go->components)
            {
                if (auto* c = kv.second.get())
                {
                    c->RootStart();
                }
            }

            go->pendingStart = false;
        }
        pendingStartObjects.clear();
    }

    // 1) MonoBehaviour.Update フェーズ
    for (auto* mb : updateList)
    {
        if (!mb || !mb->IsEnabled()) continue;
        GameObject* go = mb->gameObject;
        if (!go || !go->active)      continue;
        if (isPending(go->id))       continue;

        mb->Update();
    }

    // 2) FixedUpdate フェーズ
    for (auto* fu : fixedUpdateList)
    {
        if (!fu) continue;
        auto* c = dynamic_cast<Component*>(fu);
        if (!c || !c->IsEnabled()) continue;

        GameObject* go = c->gameObject;
        if (!go || !go->active) continue;
        if (isPending(go->id))  continue;

        fu->FixedUpdate();
    }

    // 3) 物理システム（固定刻み）
    PhysicsSystem::Step(*this, fixedDt);

    // 4) LateUpdate フェーズ
    for (auto* lu : lateUpdateList)
    {
        if (!lu) continue;
        auto* c = dynamic_cast<Component*>(lu);
        if (!c || !c->IsEnabled()) continue;

        GameObject* go = c->gameObject;
        if (!go || !go->active) continue;
        if (isPending(go->id))  continue;

        lu->LateUpdate();
    }

    // 5) Destroy キュー処理
    if (!toDestroy.empty())
    {
        ProcessDestroyQueue();
    }
}

void World::Draw()
{
    RenderSystem::Render(*this);
}

void World::ClearAll()
{
    isClearing = true;

    // OnDestroy を呼んでから全削除
    for (auto& up : objects)
    {
        if (!up) continue;
        auto& o = *up;
        for (auto& kv : o.components)
        {
            if (auto* c = kv.second.get())
            {
                c->RootOnDestroy();
            }
        }
    }

    objects.clear();
    toDestroy.clear();

    updateList.clear();
    fixedUpdateList.clear();
    lateUpdateList.clear();

    isClearing = false;
}

//====================== 登録 / 解除 ======================

namespace
{
    template<class T>
    void RemovePtr(std::vector<T*>& v, T* p)
    {
        auto it = std::find(v.begin(), v.end(), p);
        if (it != v.end())
        {
            *it = v.back();
            v.pop_back();
        }
    }
}

void World::Register(MonoBehaviour* mb)
{
    if (!mb) return;
    updateList.push_back(mb);
}

void World::Register(IFixedUpdate* fu)
{
    if (!fu) return;
    fixedUpdateList.push_back(fu);
}

void World::Register(ILateUpdate* lu)
{
    if (!lu) return;
    lateUpdateList.push_back(lu);
}

void World::Unregister(MonoBehaviour* mb)
{
    if (!mb) return;
    RemovePtr(updateList, mb);
}

void World::Unregister(IFixedUpdate* fu)
{
    if (!fu) return;
    RemovePtr(fixedUpdateList, fu);
}

void World::Unregister(ILateUpdate* lu)
{
    if (!lu) return;
    RemovePtr(lateUpdateList, lu);
}

//====================== 破壊キュー処理 ======================

void World::ProcessDestroyQueue()
{
    if (toDestroy.empty()) return;

    // ソートして重複削除
    std::sort(toDestroy.begin(), toDestroy.end());
    toDestroy.erase(std::unique(toDestroy.begin(), toDestroy.end()), toDestroy.end());

    // 1) OnDestroy 通知
    for (auto id : toDestroy)
    {
        if (auto* go = Find(id))
        {
            for (auto& kv : go->components)
            {
                if (auto* c = kv.second.get())
                {
                    c->RootOnDestroy();
                }
            }
        }
    }

    // 2) 実体削除（GameObject デストラクタでリスト解除）
    for (auto id : toDestroy)
    {
        auto it = std::find_if(
            objects.begin(), objects.end(),
            [id](const std::unique_ptr<GameObject>& p)
            {
                return p && p->id == id;
            });

        if (it != objects.end())
        {
            objects.erase(it);
        }
    }

    toDestroy.clear();
}
