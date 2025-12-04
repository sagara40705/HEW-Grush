#pragma once
#include <vector>
#include <memory>
#include <cstdint>

class GameObject;
class MonoBehaviour;
struct IFixedUpdate;
struct ILateUpdate;

// オブジェクトID
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
    bool isClearing = true;
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

    GameObject& CreateGameObject();
    void              RequestDestroy(ObjectId id);
    GameObject* Find(ObjectId id) noexcept;
    const GameObject* Find(ObjectId id) const noexcept;

    //============ ライフサイクル ============

    void Start();
    void Update();
    void Draw();
    void ClearAll();

    //============ 登録 API ============

    void Register(MonoBehaviour* mb);
    void Register(IFixedUpdate* fu);
    void Register(ILateUpdate* lu);

    void Unregister(MonoBehaviour* mb);
    void Unregister(IFixedUpdate* fu);
    void Unregister(ILateUpdate* lu);

private:
    // フェーズごとのリスト
    std::vector<MonoBehaviour*> updateList;
    std::vector<IFixedUpdate*>  fixedUpdateList;
    std::vector<ILateUpdate*>   lateUpdateList;

    void ProcessDestroyQueue();
};
