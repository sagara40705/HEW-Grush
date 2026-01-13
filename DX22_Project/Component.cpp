#include "Component.h"
#include "GameObject.h"

void Component::RootAwake()
{
    if (onAwake) return;
    onAwake = true;

    Awake();
}

// RootStart : World / GameObject 側から一度だけ呼ばれる入口
void Component::RootStart()
{
    if (onStart) return;
    onStart = true;

    // Unity ライクな順番:

    // 1) enabled な状態なら OnEnable
    if (enabled)
    {
        OnEnable();
    }

    // 2) Start
    Start();
}

// RootOnDestroy : 破棄時に World 側から呼ばれる入口
void Component::RootOnDestroy()
{
    if (onDestroy) return;
    onDestroy = true;

    // 有効だったものは OnDisable を呼んでから OnDestroy
    if (enabled)
    {
        OnDisable();
    }
    OnDestroy();
}

void Component::SetEnabled(bool v)
{
    if (enabled == v) return;

    enabled = v;

    // GameObject がまだ生存しており、Start が済んでいる場合だけ
    // OnEnable / OnDisable を呼ぶ。
    if (!gameObject) return;
    if (!onStart)   return;

    if (enabled)
    {
        OnEnable();
    }
    else
    {
        OnDisable();
    }
}

bool Component::IsActiveAndEnabled() const
{
    if (!gameObject) return false;
    return enabled && gameObject->IsActiveSelf();
}
