// EventComponent.h
#pragma once
#include "Component.h"
#include "Event.h"

/// ===========================================================
/// EventComponent
///   - 任意の GameObject にアタッチして、
///     「このオブジェクトで何か起きたときの通知ハブ」として使う。
///
///   - 現状は「引数なしイベント onEvent」だけ保持。
///   - 将来必要になれば、
///       Event<int> onDamage;
///       Event<Vector3> onHitPosition;
///     のような派生コンポーネントを定義することで拡張可能。
/// ===========================================================
class EventComponent : public Component
{
public:
    SimpleEvent onEvent; // 引数なしイベント

public:
    EventComponent() = default;
    ~EventComponent() override = default;

    void Awake() override {}
    void Start() override {}

    // 破棄時は Event 側のリスナーを全部クリア
    void OnDestroy() override;

    // 手動でイベントを発火させる（命名はお好みで Trigger / Invoke / Raise 等に変更可）
    void Trigger();
};
