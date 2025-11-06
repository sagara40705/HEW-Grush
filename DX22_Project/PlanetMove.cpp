// PlanetMover.cpp
#include "PlanetMove.h"
#include "Transform.h"
#include "Input.h"
#include "GameObject.h"
#include <DirectXMath.h>
using namespace DirectX;

static inline float Sign(bool pos, bool neg) { return (pos ? 1.f : 0.f) - (neg ? 1.f : 0.f); }

void PlanetMove::Start()
{
    t = Parent->TryGetComponent<Transform>();
    // プレイヤーTransformは外からセットするか、起動時にWorld走査で拾ってください
    // ここでは仮に既に設定済み想定
}

void PlanetMove::Update() {
    constexpr float DT = 1.f / 60.f;

    // 入力（WASD）: 前後=Z, 左右=X
    const float ix = Sign(IsKeyPress('A'), IsKeyPress('D'));
    const float iz = Sign(IsKeyPress('S'), IsKeyPress('W'));
    if (ix == 0.f && iz == 0.f) return;

    // プレイヤーは常に頂点へ固定
    if (playerT) { playerT->SetPosition({ 0, radius, 0 }); }

    // 目標移動方向（ワールドXZ基準）
    XMFLOAT3 moveDir = { ix, 0.f, iz };
    // 正規化
    float len = std::sqrt(moveDir.x * moveDir.x + moveDir.z * moveDir.z);
    if (len < 1e-6f) return;
    moveDir.x /= len; moveDir.z /= len;

    // 角度 = s/R
    const float angleDeg = (moveSpeed * DT / radius) * 180.f / 3.14159265f;

    // 近似: X回りとZ回りの合成で“頂点移動の逆回転”を実現
    // 前に進む(iz>0) → 惑星を +X 回りに回すと、地面が手前から奥へ動き、プレイヤーは頂点のまま前進したように見える
    const float rotX = +moveSpeed * DT / radius * (iz); // ラジアン
    const float rotZ = -moveSpeed * DT / radius * (ix); // ラジアン

    // Transformがラジアン入力のAxis回転を持つならそれを使う。なければ度でAddRotation近似。
    // ここでは度数のAddRotation近似
    t->AddRotate({ rotX * 180.f / 3.14159265f, 0.f, rotZ * 180.f / 3.14159265f });
}
