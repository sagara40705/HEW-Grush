#pragma once
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cassert>
#include "Component.h"

using ObjectId = std::uint32_t;
class World;

enum class Tag : uint8_t
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
	bool active = true;
    std::unordered_map<std::type_index, std::unique_ptr<Component>> components{};
    World* world = nullptr;
    Tag tag = Tag::None;

    ~GameObject() {}

    // ’Ç‰Á
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

        return *raw;
    }

    // Žæ“¾
    template<class T>
    T& GetComponent() 
    {
        static_assert(std::is_base_of_v<Component, T>);
        auto it = components.find(std::type_index(typeid(T)));
        assert(it != components.end());
        return *static_cast<T*>(it->second.get());
    }

    template<class T>
    T* TryGetComponent() noexcept
    {
        static_assert(std::is_base_of_v<Component, T>);
        auto it = components.find(std::type_index(typeid(T)));
        return (it == components.end()) ? nullptr : static_cast<T*>(it->second.get());
    }

    void Destroy(float time = 0.f);

    void SetTag(Tag t) { tag = t; }
    bool CompareTag(Tag t) { return (tag == t); }

    GameObject() = default;
    GameObject(const GameObject&) = delete;
    GameObject& operator=(const GameObject&) = delete;
    GameObject(GameObject&&) noexcept = default;
    GameObject& operator=(GameObject&&) noexcept = default;
};