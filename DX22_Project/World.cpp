#include "World.h"
#include "GameObject.h"
#include "Component.h"
#include "Transform.h"
#include "RenderSystem.h"
#include "PhysicsSystem.h"
#include "BoxCollider.h"
//#include "DebugLog.h"

#include <algorithm>

//====================== ctor / dtor ======================

World::World() = default;
World::~World() = default;

//====================== �����E�j���E���� ======================

void World::RequestStart(GameObject* go)
{
    if (!go) return;
    pendingStartObjects.push_back(go);
}

GameObject& World::CreateGameObject(const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
    auto obj = std::make_unique<GameObject>();
    obj->id = nextId++;
    obj->world = this;

    GameObject* raw = obj.get();
    // ★ 必ず Transform を追加
    {
        Transform& t = raw->AddComponent<Transform>(pos, rot, scale);
        raw->transform = &t;   // GameObject が Transform* を持つ
    }

    objects.emplace_back(std::move(obj));

    // �� �V�K������ Start �Ώ�
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

GameObject* World::FindWithTag(Tag tag)
{
    for (auto& up : objects)
    {
        if (!up) continue;
        GameObject* go = up.get();

        // Destroy 予約済みは除外
        if (go->isDestroyScheduled) continue;

        if (go->CompareTag(tag))
            return go;
    }
    return nullptr;
}

//====================== ���C�t�T�C�N�� ======================

void World::Start()
{

    // すべての GameObject に対して Start を流す
    // 今フレームで処理する分を退避
    std::vector<GameObject*> starts = std::move(pendingStartObjects);
    pendingStartObjects.clear();

    for (auto& up : starts)
    {
        if (!up) continue;
        //if (!up->active) continue;
        up->CallStart();
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

        up->DestroyTimerUpdate(fixedDt);
    }

    // 0) Start �҂� GameObject �� Start �����s
    if (!pendingStartObjects.empty())
    {
        // 今フレームで処理する分を退避
        std::vector<GameObject*> starts = std::move(pendingStartObjects);
        pendingStartObjects.clear();

        for (auto* go : starts)
        {
            if (!go) continue;
            //if (!go->active) continue;

            go->CallStart();
        }
    }

    // 1) MonoBehaviour.Update �t�F�[�Y
    for (auto* mb : updateList)
    {
        if (!mb || !mb->IsEnabled()) continue;
        GameObject* go = mb->gameObject;
        if (!go || !go->active)      continue;
        if (isPending(go->id))       continue;

        mb->Update();
    }

    // 2) FixedUpdate �t�F�[�Y
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

    // 3) �����V�X�e���i�Œ荏�݁j
    PhysicsSystem::Step(*this, fixedDt);

    // 4) LateUpdate �t�F�[�Y
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

    // 5) Destroy �L���[����
    if (!toDestroy.empty())
    {
        ProcessDestroyQueue();
    }
}

void World::Draw()
{
    Render(*this);
}

void World::ClearAll()
{
    isClearing = true;

    // OnDestroy ���Ă�ł���S�폜
    for (auto& up : objects)
    {
        if (!up) continue;
        
        up->CallDestroy();
    }

    objects.clear();
    toDestroy.clear();

    updateList.clear();
    fixedUpdateList.clear();
    lateUpdateList.clear();
    rendererList.clear();
    colliderList.clear();

    isClearing = false;
}

//====================== �o�^ / ���� ======================

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
void World::Register(Renderer* r)
{
    if (!r) return;
    rendererList.push_back(r);
}
void World::Register(Collider* c)
{
    if (!c) return;
    colliderList.push_back(c);
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
void World::Unregister(Renderer* r)
{
    if (!r) return;
    RemovePtr(rendererList, r);
}
void World::Unregister(Collider* c)
{
    if (!c) return;
    RemovePtr(colliderList, c);
}

void World::DebugDrawColliders() const
{
    // BoxCollider 用レイヤーだけ消す
    LineManager::Clear(LineManager::LineGroup::BoxCollider);

    // すべての GameObject から BoxCollider を探す
    for (auto& up : objects)
    {
        if (!up) continue;
        GameObject* go = up.get();
        if (!go) continue;

        // 非アクティブなオブジェクトはスキップ
        if (!go->IsActiveInHierarchy()) continue;

        // BoxCollider を取得
        auto* bc = go->GetComponent<BoxCollider>();
        if (!bc) continue;

        // 無効コンポーネントならスキップ
        if (!bc->IsEnabled()) continue;  // BoxCollider は Component 継承前提

        // ライン登録
        LineManager::AddBoxCollider(bc);
    }

    // ライン描画（BoxCollider レイヤーのみ）
    LineManager::Draw(LineManager::LineGroup::BoxCollider);
}

//====================== �j��L���[���� ======================

void World::ProcessDestroyQueue()
{
    if (toDestroy.empty()) return;

    // �\�[�g���ďd���폜
    std::sort(toDestroy.begin(), toDestroy.end());
    toDestroy.erase(std::unique(toDestroy.begin(), toDestroy.end()), toDestroy.end());

    // 1) OnDestroy �ʒm
    for (auto id : toDestroy)
    {
        if (auto* go = Find(id))
        {
            go->CallDestroy();
        }
    }

    // 2) ���̍폜�iGameObject �f�X�g���N�^�Ń��X�g�����j
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
