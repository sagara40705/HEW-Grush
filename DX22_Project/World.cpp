#include "World.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "DebugLog.h"

GameObject& World::CreateGameObject()
{
    auto obj = std::make_unique<GameObject>();
    obj->id = nextId++;
    obj->world = this;
    objects.emplace_back(std::move(obj));
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
    for (auto& up : objects) {
        if (up && up->id == id) return up.get();
    }
    return nullptr;
}

const GameObject* World::Find(ObjectId id) const noexcept
{
    for (auto& up : objects) {
        if (up && up->id == id) return up.get();
    }
    return nullptr;
}

void World::Start()
{
    for (auto& up : objects) 
    {
        if (!up) continue;
        auto& o = *up;
        // すべてのコンポーネントに対して Start 呼び出し（既存方針）
        for (auto& kv : o.components)
        {
            if (auto* c = kv.second.get())
            {
                c->Start();
            }
        }
    }
}

void World::Update()
{
    // 破壊予定をスキップ
    auto isPending = [this](ObjectId id) 
    {
        return std::find(toDestroy.begin(), toDestroy.end(), id) != toDestroy.end();
    };

    for (auto& up : objects)
    {
        if (!up) continue;
        auto& o = *up;
        if (!o.active || isPending(o.id)) continue;

        for (auto& kv : o.components)
        {
            Component* c = kv.second.get();
            if (!c || !c->IsEnabled()) continue;

            if (auto* mb = dynamic_cast<MonoBehaviour*>(c)) 
            {
                mb->Update();
            }
        }
    }

    PhysicsSystem::Step(*this, 1.0f / 60.0f);

    if (toDestroy.empty()) return;
    //破壊処理
    ProcessDestroyQueue();
}

void World::Draw()
{
    RenderSystem::Render(*this);
}

void World::ClearAll()
{
    // OnDestroy を呼んでから全削除
    for (auto& up : objects) 
    {
        if (!up) continue;
        for (auto& kv : up->components) 
        {
            if (kv.second) kv.second->OnDestroy();
        }
    }
    // 2) 実体を破棄
    objects.clear();
    toDestroy.clear();
}

void World::ProcessDestroyQueue()
{
    if (toDestroy.empty()) return;

    std::sort(toDestroy.begin(), toDestroy.end());
    toDestroy.erase(std::unique(toDestroy.begin(), toDestroy.end()), toDestroy.end());

    // 1) OnWillDestroy（まだ参照している可能性があるので先に通知）
    for (auto id : toDestroy) 
    {
        if (auto* go = Find(id)) {
            for (auto& kv : go->components) {
                if (kv.second) kv.second->OnWillDestroy();
            }
        }
    }

    // 2) OnDestroy → 実体削除
    for (auto id : toDestroy) {
        auto it = std::find_if(objects.begin(), objects.end(),
            [id](const std::unique_ptr<GameObject>& p) { return p && p->id == id; });

        if (it != objects.end()) {
            // OnDestroy
            for (auto& kv : (*it)->components) {
                if (kv.second) kv.second->OnDestroy();
            }
            // unique_ptr をベクタから取り除く（他要素の「指す先アドレス」は不変）

            objects.erase(it);
        }
    }


    toDestroy.clear();
}

