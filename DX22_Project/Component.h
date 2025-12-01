#pragma once
#include <type_traits>
#include "Vector3.h"

class GameObject;

//====================== 基底コンポーネント ======================
class Component
{
public:
    GameObject* gameObject = nullptr;  // 所有者

private:
    bool enabled = true;  // 有効フラグ (Unity の enabled 相当)
    bool onAwake = false;
    bool onStart = false; // Start 済みか
    bool onDestroy = false; // OnDestroy 済みか

public:
    virtual ~Component() = default;

    // World / GameObject から呼ばれるライフサイクル入口
    void RootAwake();
    void RootStart();
    void RootOnDestroy();

public:
    //=== Unity 風ライフサイクル ===
    virtual void Awake() {} // 初回開始前に一度
    virtual void OnEnable() {} // enabled になったとき
    virtual void OnDisable() {} // disabled になったとき
    virtual void Start() {} // Awake のあと、最初のフレームで一度
    virtual void OnDestroy() {} // 破棄時に一度

public:
    //=== 有効状態 ===
    void SetEnabled(bool v);
    bool IsEnabled() const { return enabled; }

    // GameObject.active と合成した状態 (Unity の isActiveAndEnabled 相当)
    bool IsActiveAndEnabled() const;

    // 便利関数: owner 経由で他コンポーネントにアクセス
    template<class T>
    T* GetComponent() noexcept;
};

//====================== 通常スクリプト用 ======================

class MonoBehaviour : public Component
{
public:
    // 必要に応じて override:
    // Awake(), OnEnable(), Start(), OnDisable(), OnDestroy() は
    // Component 側で virtual になっているのでそのまま override 可能。

    virtual void Update() {}     // ロジック用の毎フレーム処理
    virtual void LateUpdate() {} // 物理後の処理
    virtual void Draw() {}       // 簡易描画用
};

//====================== FixedUpdate / LateUpdate 用 ======================

// 物理タイミングで呼びたいもの（FixedUpdate 相当）
struct IFixedUpdate
{
    virtual ~IFixedUpdate() = default;
    virtual void FixedUpdate() = 0;
};

// すべての移動・物理後に呼びたいもの（LateUpdate 相当）
struct ILateUpdate
{
    virtual ~ILateUpdate() = default;
    virtual void LateUpdate() = 0;
};

//====================== 描画専用ベースクラス ======================

class Renderer : public Component
{
private:
    bool visible = true; // 描画可否
    int  sortingOrder = 0;    // Unity の sortingOrder 相当

public:
    virtual ~Renderer() = default;

    void SetVisible(bool v) { visible = v; }
    bool IsVisible() const { return visible && IsActiveAndEnabled(); }

    void SetSortingOrder(int order) { sortingOrder = order; }
    int  GetSortingOrder() const { return sortingOrder; }

    // 各レンダラーはここを実装して描画処理を書く
    virtual void Render() = 0;
};

struct CollisionInfo
{
    GameObject* other = nullptr;
    DirectX::XMFLOAT3 normal{ 0,0,0 }; // 自分←相手 の押し戻し法線
    float penetration = 0.0f;
};

class CollisionListener
{
public:
    virtual void OnCollisionEnter(const CollisionInfo& c) {}
    virtual void OnCollisionStay(const CollisionInfo& c) {}
    virtual void OnCollisionExit(GameObject& other) {}
};

struct TriggerInfo
{
    GameObject* other = nullptr;
};

class TriggerListener
{
public:
    virtual ~TriggerListener() = default;
    virtual void OnTriggerEnter(const TriggerInfo& info) {}
    virtual void OnTriggerStay(const TriggerInfo& info) {}
    virtual void OnTriggerExit(GameObject& other) {}
};

class Transform;
class Camera;
class MeshFilter;
class MeshRenderer;
class Collider;
class Rigidbody;
class Canvas;
class UiSpriteRenderer;
class Sprite3DRenderer;
class SpriteAnimation;
class Animator;
