#include "GameObject.h"
#include "Component.h"
#include "World.h"

// Transform を別ファイルにしている場合
// #include "Transform.h"

GameObject::~GameObject()
{
    if (!world) return;

    // World のリストから登録解除
    for (auto& kv : components)
    {
        Component* c = kv.second.get();
        if (!c) continue;

        if (auto* mb = dynamic_cast<MonoBehaviour*>(c))
            world->Unregister(mb);

        if (auto* fu = dynamic_cast<IFixedUpdate*>(c))
            world->Unregister(fu);

        if (auto* lu = dynamic_cast<ILateUpdate*>(c))
            world->Unregister(lu);

        if (auto* r = dynamic_cast<Renderer*>(c))
            world->Unregister(r);

        if (auto* co = dynamic_cast<Collider*>(c))
            world->Unregister(co);
    }
}

void GameObject::SetActive(bool v)
{
    if (active == v) return;
    active = v;

    // active が変わった後、enabled 状態と合わせて
    // OnEnable/OnDisable を呼ぶ必要がある。
    for (auto& pair : components)
    {
        auto& comp = pair.second;
        // Start 前のものは触らない
        // （Start 内で初期化前に OnEnable/OnDisable が来るのを避ける）
        // RootStart() 内で Awake → OnEnable → Start が呼ばれる設計なので、
        // このタイミングでは onStart が true のものだけに反応させたい場合は
        // Component 側のフラグを public な getter 経由で参照するようにしてもよい。
        if (comp->IsActiveAndEnabled())
        {
            comp->OnEnable();
        }
        else
        {
            comp->OnDisable();
        }
    }

    // 子に伝播
    for (auto& c : children)
    {
        c->SetActive(v);
    }
}

bool GameObject::IsActiveInHierarchy() const
{
    if (!active) return false;

    const GameObject* p = parent;
    while (p)
    {
        if (!p->active) return false;
        p = p->parent;
    }
    return true;
}

GameObject& GameObject::CreateChild(const Vector3& pos, const Vector3& rot, const Vector3& scale)
{
    GameObject& child = world->CreateGameObject(pos, rot, scale);

    child.parent = this;
    children.push_back(&child);

    return child;
}

//-----------------------------
// 親子リンク解除
//-----------------------------
void GameObject::DetachParent()
{
    if (!parent) return;

    auto& siblings = parent->children;
    siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());

    parent = nullptr;

    // Transform の再計算（ローカル値はそのまま、親なしとして再ビルド）
    if (transform)
    {
        const auto p = transform->GetPosition();
        const auto r = transform->GetRotation();
        const auto s = transform->GetScale();
        transform->SetTransform(p, r, s);
    }
}

void GameObject::DetachChildren()
{
    for (auto* c : children)
    {
        if (!c) continue;
        if (c->parent == this)
        {
            c->parent = nullptr;

            if (c->transform)
            {
                const auto p = c->transform->GetPosition();
                const auto r = c->transform->GetRotation();
                const auto s = c->transform->GetScale();
                c->transform->SetTransform(p, r, s);
            }
        }
    }
    children.clear();
}

void GameObject::DetachAllRelations()
{
    DetachParent();
    DestroyChildren();
}

void GameObject::DestroyChildren()
{
    // 安全のためコピーを作る（Destroy 呼び出しでリストが変わる可能性がある）
    auto copy = children;

    for (auto* c : copy)
    {
        if (!c) continue;
        if (c->parent == this)
        {
            // 子を破棄予約
            c->Destroy();

            // 親リンク解除（破壊までの1フレームでTransform依存処理が走らないように）
            c->parent = nullptr;
        }
    }

    // 全部削除
    children.clear();
}

void GameObject::CallStart()
{
    if (!pendingStart) return;
    pendingStart = false;

    for (auto& pair : components)
    {
        pair.second->RootStart();
    }
}

void GameObject::CallDestroy()
{
    DetachAllRelations();

    for (auto& pair : components)
    {
        pair.second->RootOnDestroy();
    }
}

void GameObject::Destroy(float time)
{
    if (isDestroyScheduled) return;
    isDestroyScheduled = true;
    destroyTimer = time;

    // ここは World の既存仕様に合わせて書き換えてください。
    // 例: すぐ消す場合
    if (time <= 0.0f)
    {
        world->RequestDestroy(id); // ← 実際の関数名に合わせて修正
    }
    // 時間差破棄にする場合は World 側で「DestroyTimer 管理リスト」を持つか、
    // World::Update から GameObject::DestroyTimerUpdate(dt) を呼ぶようにする。
}

void GameObject::DestroyTimerUpdate(float dt)
{
    if (!isDestroyScheduled) return;
    if (destroyTimer <= 0.0f) return;

    destroyTimer -= dt;
    if (destroyTimer <= 0.0f)
    {
        world->RequestDestroy(id); // ここも既存仕様に合わせて修正
    }
}

GameObject* GameObject::FindChildWithTag(Tag tag)
{
    for (GameObject* child : children)
    {
        if (child->CompareTag(tag)) return child;
    }
    return nullptr;
}
