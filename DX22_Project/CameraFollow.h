#pragma once
#include "Component.h"
#include "Transform.h"
#include <DirectXMath.h>

class CameraFollow final : public MonoBehaviour
{
public:
    // ====== 調整用定数 ======
    static constexpr DirectX::XMFLOAT3 OFFSET = { 0.0f, 3.0f, -6.0f }; // プレイヤーからの相対位置

public:
    // ターゲットをTransformで指定
    explicit CameraFollow(Transform* target)
        : target_(target)
    {}

    void Start() override;
    void Update() override;
    void OnDestroy() override;

private:
    Transform* self_ = nullptr;  // カメラ自身のTransform
    Transform* target_ = nullptr;  // 追従対象（プレイヤー）
};
