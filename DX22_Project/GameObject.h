#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>
#include <cstdint>
#include <cassert>
#include <type_traits>
#include <utility>

#include "World.h"
#include "Component.h"
#include "Transform.h"
#include "Collider.h"

//====================== Tag / ID ======================

enum class Tag : std::uint8_t
{
    None,
    GameManager,
    GaugeManager,
    ScoreManager,
    TimerUI,
    CamreraPivot,
    MainCamera,
    Player,
    Hammer,
    Enemy,
    Glass,
    ActionUI,
    ResultTimerUI,
	GaugeUI,
    // 必要に応じて追加
};

// 前方宣言（必要なら）
class Transform;

//=========================================================
// GameObject 
//=========================================================
class GameObject
{
public:
    ObjectId   id = 0;
    World* world = nullptr;
    Transform* transform = nullptr; // 必ず存在させる
    bool       active = true;      // activeSelf 相当
    bool  pendingStart = true; // Start 未実行
    bool  isDestroyScheduled = false;
   
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components{};
private:
    Tag  tag = Tag::None;
    float destroyTimer = 0.0f;

    std::vector<GameObject*> children{};
    GameObject* parent = nullptr;

public:
    GameObject() {}
    ~GameObject();

    //====================== コンポーネント追加 ======================
    template<class T, class... Args>
    T& AddComponent(Args&&... args);

    //====================== コンポーネント取得 ======================
    template<class T>
    T* GetComponent() noexcept;

    //====================== active ======================

    void SetActive(bool v);
    bool IsActiveSelf() const { return active; }
    bool IsActiveInHierarchy() const;

    //====================== 子オブジェクト ======================

    GameObject& CreateChild(const Vector3& pos = Vector3::zero(), const Vector3& rot = Vector3::zero(), const Vector3& scale = Vector3::one());

    GameObject* GetParent() const { return parent; }
    const std::vector<GameObject*>& GetChildren() const { return children; }

    // 親子リンク操作（親子関係を消す用）
    void DetachParent();         // 自分を親から外す
    void DetachChildren();       // 全ての子を外す（子の parent を nullptr に）
    void DetachAllRelations();   // 親・子の両方を全部切る
    void DestroyChildren();

    //====================== Start / Destroy（World から呼ばれる） ======================

    void CallStart();   // World::Update 内から呼ぶ
    void CallDestroy(); // World::DestroyObject 内などから呼ぶ

    //====================== 破棄予約 ======================

    void Destroy(float time = 0.f);
    void DestroyTimerUpdate(float dt);

    //====================== Tag ユーティリティ ======================

    void SetTag(Tag t) { tag = t; }
    bool CompareTag(Tag t) const { return tag == t; }
    GameObject* FindWithTag(Tag tag) { return world->FindWithTag(tag); }
    GameObject* FindChildWithTag(Tag tag);
};

//=========================================================
// ここからテンプレート関数の実装
//=========================================================


//---- AddComponent ----

template<class T, class... Args>
T& GameObject::AddComponent(Args&&... args)
{
    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

    const std::type_index key = typeid(T);
    auto it = components.find(key);
    assert(it == components.end() && "Component already exists on this GameObject");

    auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
    T* raw = ptr.get();
    raw->gameObject = this;

    components.emplace(key, std::move(ptr));

    //--------- ライフサイクル ----------
    raw->RootAwake();
    pendingStart = true; // World 側で次フレーム CallStart() するためのフラグ

    //--------- 登録リスト方式 ----------
    if (auto* mb = dynamic_cast<MonoBehaviour*>(raw))
    {
        world->Register(mb);
    }
    if (auto* fu = dynamic_cast<IFixedUpdate*>(raw))
    {
        world->Register(fu);
    }
    if (auto* lu = dynamic_cast<ILateUpdate*>(raw))
    {
        world->Register(lu);
    }
    if (auto* r = dynamic_cast<Renderer*>(raw))
    {
        world->Register(r);
    }
    if (auto* c = dynamic_cast<Collider*>(raw))
    {
        world->Register(c);
    }

    return *raw;
}

//---- GetComponent / TryGetComponent ----
template<class T>
T* GameObject::GetComponent() noexcept
{
    static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");

    const std::type_index key = typeid(T);
    auto it = components.find(key);
    if (it == components.end()) return nullptr;
    return static_cast<T*>(it->second.get());
}

//=========================================================
// Component 側テンプレートの実装
// ここなら GameObject が完全型なので OK
//=========================================================

template<class T>
T* Component::GetComponent() noexcept
{
    return gameObject->GetComponent<T>();
}
