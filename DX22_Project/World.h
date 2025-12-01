#pragma once
#include <vector>
#include <memory>
#include <cstdint>
#include "Vector3.h"

class GameObject;
class MonoBehaviour;
struct IFixedUpdate;
struct ILateUpdate;
class Renderer;                 // ★ 追加: 描画用ベースクラスの前方宣言
class Collider;

// オブジェクトID
enum class Tag : std::uint8_t;
using ObjectId = std::uint32_t;

// World 全体の管理
class World
{
public:
    // 実体一覧
    std::vector<std::unique_ptr<GameObject>> objects;
    // 破棄予約
    std::vector<ObjectId>  toDestroy{};
    ObjectId               nextId = 1;
    bool                   isClearing = true;

private:
    std::vector<GameObject*> pendingStartObjects;

public:
    World();
    ~World();

    World(const World&) = delete;
    World& operator=(const World&) = delete;
    World(World&&) = delete;
    World& operator=(World&&) = delete;

    // Start 予約 API
    void RequestStart(GameObject* go);

    //============ 生成・破棄・検索 ============

    GameObject& CreateGameObject(const Vector3& pos = Vector3::zero(), const Vector3& rot = Vector3::zero(),const Vector3& scale = Vector3::one());
    void             RequestDestroy(ObjectId id);
    GameObject* Find(ObjectId id) noexcept;
    const GameObject* Find(ObjectId id) const noexcept;
    GameObject* FindWithTag(Tag tag);

    //============ ライフサイクル ============

    void Start();
    void Update();
    void Draw();
    void ClearAll();

    //============ 登録 API ============

    void Register(MonoBehaviour* mb);
    void Register(IFixedUpdate* fu);
    void Register(ILateUpdate* lu);
    void Register(Renderer* r);        // ★ 追加: Renderer の登録
    void Register(Collider* c);

    void Unregister(MonoBehaviour* mb);
    void Unregister(IFixedUpdate* fu);
    void Unregister(ILateUpdate* lu);
    void Unregister(Renderer* r);      // ★ 追加: Renderer の登録解除
    void Unregister(Collider* c);

    // RenderSystem から参照する用（読み取り専用）
    const std::vector<Renderer*>& GetRenderers() const { return rendererList; }
    // RenderSystem から参照する用（読み取り専用）
    const std::vector<Collider*>& GetColliders() const { return colliderList; }
    // ★ 追加: デバッグ用コライダー描画
    void DebugDrawColliders() const;

private:
    // フェーズごとのリスト
    std::vector<MonoBehaviour*> updateList;
    std::vector<IFixedUpdate*>  fixedUpdateList;
    std::vector<ILateUpdate*>   lateUpdateList;
    std::vector<Renderer*>      rendererList;  
    std::vector<Collider*>      colliderList;

    void ProcessDestroyQueue();
};