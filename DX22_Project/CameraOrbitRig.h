#pragma once
#include "Component.h"
#include "Transform.h"
#include "GameObject.h"

// 親（=CameraPivot。Playerの直下）に付ける。左右入力でYawだけ回す。
class CameraOrbitRig final : public MonoBehaviour
{
public:
    struct Settings { float yawSpeedDeg = 120.0f; };
    explicit CameraOrbitRig(const Settings& s = {}) : cfg_(s) {}
    void Start() override { pivot_ = gameObject->TryGetComponent<Transform>(); }
    void Update() override;

    // 外部（マウス/右スティック等）から直接加算したい場合
    void AddYawDegrees(float d) { if (pivot_) pivot_->AddRotate({ 0, d, 0 }); }

private:
    Transform* pivot_{};
    Settings   cfg_{};

    void ApplyInput_();
};
