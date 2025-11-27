#pragma once
#include "DebugLog.h"

class GameObject;

//====================== 基底コンポーネント ======================

class Component
{
public:
    GameObject* gameObject = nullptr; // 所有者

private:
    bool enabled = true; // 有効フラグ
    bool onStart = false;
    bool onDestroy = false;

public:
    virtual ~Component() = default;

    // World から呼ぶライフサイクル入口
    void RootStart();
    void RootOnDestroy();

protected:
    // ユーザーが必要に応じてオーバーライド
    virtual void Start() {}
    virtual void OnDestroy() {}

public:
    void SetEnabled(bool v) { enabled = v; }
    bool IsEnabled() const { return enabled; }
};

//====================== 通常スクリプト用 ======================

class MonoBehaviour : public Component
{
public:
    virtual void Update() {}
    virtual void Draw() {}
};

//====================== FixedUpdate / LateUpdate 用インターフェース ======================

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

//（必要なら）他コンポーネントの前方宣言
class Transform;
class Camera;
class MeshFilter;
class MeshRender;
class PlayerAction;
class Rigidbody;
