#pragma once
#include <unordered_map>
#include <functional>
#include <typeindex>

#include "Vector3.h"
#include "World.h"
#include "GameObject.h"
#include "Transform.h"

// Prefab の種類
enum class PrefabId : uint8_t
{
    Player,
    Ground,
    GlassBlock,
    Swing,
    Benti,
    Ufo,
    ProtPlay,
    Count
};

// enum class を unordered_map のキーに使うためのハッシュ
namespace std
{
    template<>
    struct hash<PrefabId>
    {
        size_t operator()(PrefabId v) const noexcept
        {
            return static_cast<size_t>(v);
        }
    };
}

// 登録された Prefab 情報
class Prefab
{
public:
    using BuilderFunc = std::function<void(GameObject&)>;

private:
    static std::unordered_map<PrefabId, BuilderFunc> s_table;

public:
    // 登録：起動時に一度だけ呼ぶ
    static void Register(PrefabId id, const BuilderFunc& builder);

    // 生成：World 内に GameObject を作り、Transform を設定してから Builder を呼ぶ
    static GameObject& Instantiate(World& world, PrefabId id, const Vector3& pos = Vector3::zero(), const Vector3& rot = Vector3::zero(), const Vector3& scale = Vector3::one());
};

void RegisterAllPrefabs();