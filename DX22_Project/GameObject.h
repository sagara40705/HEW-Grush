#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cstdint>
#include <cassert>
#include <type_traits>
#include <utility>
#include "Component.h"
#include "World.h"

// タグ（既存仕様に合わせておく）
enum class Tag : std::uint8_t
{
    None,
    Player,
    Weapon,
    Glass,
    Ground,
};

class GameObject
{
public:
    ObjectId id = 0;
    bool     active = true;

    std::unordered_map<std::type_index, std::unique_ptr<Component>> components{};
    World* world = nullptr;
    Tag    tag = Tag::None;
    bool pendingStart = false;

public:
    GameObject() = default;
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) noexcept = default;
    GameObject& operator=(GameObject&&) noexcept = default;

    ~GameObject(); // 登録解除のため cpp で定義

    //====================== コンポーネント追加 ======================

    template<class T, class... Args>
    T& AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        const std::type_index key = typeid(T);

        auto it = components.find(key);
        assert(it == components.end() && "Component already exists on this GameObject");

        auto ptr = std::make_unique<T>(std::forward<Args>(args)...);
        ptr->gameObject = this;

        T* raw = ptr.get();
        components.emplace(key, std::move(ptr));

        // World へ登録（MonoBehaviour / IFixedUpdate / ILateUpdate）
        if (world)
        {
            if (auto* mb = dynamic_cast<MonoBehaviour*>(raw))
                world->Register(mb);

            if (auto* fu = dynamic_cast<IFixedUpdate*>(raw))
                world->Register(fu);

            if (auto* lu = dynamic_cast<ILateUpdate*>(raw))
                world->Register(lu);
        }

        if (!pendingStart)
        {
            pendingStart = true;
            world->RequestStart(this);
        }

        return *raw;
    }

    //====================== コンポーネント取得 ======================

    template<class T>
    T& GetComponent()
    {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        auto it = components.find(std::type_index(typeid(T)));
        assert(it != components.end() && "Component not found");
        return *static_cast<T*>(it->second.get());
    }

    template<class T>
    T* TryGetComponent() noexcept
    {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Component");
        auto it = components.find(std::type_index(typeid(T)));
        return (it == components.end()) ? nullptr : static_cast<T*>(it->second.get());
    }

    //====================== 破棄予約 ======================

    void Destroy(float time = 0.f);

    //====================== Tag ユーティリティ ======================

    void SetTag(Tag t) { tag = t; }
    bool CompareTag(Tag t)    const { return tag == t; }
};
