#include "Transform.h"
#include "GameObject.h"
using namespace DirectX;

Transform::Transform(Vector3 p, Vector3 r, Vector3 s)
    : pos(p), rotate(r), scale(s)
{
    XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
    dirty = true;
}

//-----------------------------
// SetParent
//-----------------------------
// ※実際の親子は GameObject 側で持つ。
//   ここでは keepWorld = true のときにローカルTRSを再計算して
//   「見た目のワールド姿勢を維持」するためだけに使う。
void Transform::SetParent(Transform* newParent, bool keepWorld)
{
    if (!keepWorld || !newParent)
    {
        // 親子変更だけ行う場合は、GameObject 側の処理の後で
        // MarkDirtyRecursive しておけば十分
        MarkDirtyRecursive();
        return;
    }

    // 現在のワールド行列を保持
    XMMATRIX curWorld = XMLoadFloat4x4(&GetMatrix());

    // 新しい親のワールド行列
    XMMATRIX P = XMLoadFloat4x4(&newParent->GetMatrix());
    XMVECTOR det{};
    XMMATRIX Pinv = XMMatrixInverse(&det, P);

    // L = World * inverse(ParentWorld)
    XMMATRIX L = XMMatrixMultiply(curWorld, Pinv);

    // 分解（S,Q,T）
    XMVECTOR S, RQ, T;
    if (XMMatrixDecompose(&S, &RQ, &T, L))
    {
        XMFLOAT3 pf{}, sf{};
        XMFLOAT4 rqf{};
        XMStoreFloat3(&pf, T);
        XMStoreFloat3(&sf, S);
        XMStoreFloat4(&rqf, RQ);

        // クォータニオン → オイラー（X→Y→Z）簡易変換
        XMMATRIX Rm = XMMatrixRotationQuaternion(RQ);

        float pitch, yaw, roll;
        {
            // Row-major 前提
            float r21 = Rm.r[2].m128_f32[1];

            pitch = std::asin(-r21);
            yaw = std::atan2(Rm.r[0].m128_f32[2], Rm.r[2].m128_f32[2]);
            roll = std::atan2(Rm.r[1].m128_f32[0], Rm.r[1].m128_f32[1]);
        }

        pos = Vector3(pf.x, pf.y, pf.z);
        scale = Vector3(sf.x, sf.y, sf.z);
        rotate = Vector3(
            XMConvertToDegrees(pitch),
            XMConvertToDegrees(yaw),
            XMConvertToDegrees(roll));
    }

    MarkDirtyRecursive();
}

//-----------------------------
// 方向ベクトル（world）
//-----------------------------
DirectX::XMFLOAT3 Transform::Forward()
{
    RebuildIfDirty();
    XMFLOAT3 f{ worldMatrix._31, worldMatrix._32, worldMatrix._33 };
    XMVECTOR v = XMVector3Normalize(XMLoadFloat3(&f));
    XMStoreFloat3(&f, v);
    return f;
}

DirectX::XMFLOAT3 Transform::Up()
{
    RebuildIfDirty();
    XMFLOAT3 u{ worldMatrix._21, worldMatrix._22, worldMatrix._23 };
    XMVECTOR v = XMVector3Normalize(XMLoadFloat3(&u));
    XMStoreFloat3(&u, v);
    return u;
}

DirectX::XMFLOAT3 Transform::Right()
{
    RebuildIfDirty();
    XMFLOAT3 r{ worldMatrix._11, worldMatrix._12, worldMatrix._13 };
    XMVECTOR v = XMVector3Normalize(XMLoadFloat3(&r));
    XMStoreFloat3(&r, v);
    return r;
}

//-----------------------------
// 回転クォータニオン
//-----------------------------
XMVECTOR Transform::GetRotationQuaternionXM() const
{
    const float pitch = XMConvertToRadians(rotate.x);
    const float yaw = XMConvertToRadians(rotate.y);
    const float roll = XMConvertToRadians(rotate.z);
    // X→Y→Z (pitch, yaw, roll)
    return XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
}

XMFLOAT4 Transform::GetRotationQuaternion() const
{
    XMFLOAT4 out{};
    XMStoreFloat4(&out, GetRotationQuaternionXM());
    return out;
}

//-----------------------------
// 行列／ワールド座標
//-----------------------------
const XMFLOAT4X4& Transform::GetMatrix()
{
    RebuildIfDirty();
    return worldMatrix;
}

XMFLOAT3 Transform::GetWorldPosition()
{
    RebuildIfDirty();
    return XMFLOAT3(worldMatrix._41, worldMatrix._42, worldMatrix._43);
}

//-----------------------------
// 行列再計算
//-----------------------------
void Transform::RebuildIfDirty()
{
    if (!dirty) return;
    dirty = false;

    // ローカル TRS → ローカル行列
    XMVECTOR S = XMVectorSet(scale.x, scale.y, scale.z, 0.0f);
    XMVECTOR Q = GetRotationQuaternionXM();
    XMVECTOR T = XMVectorSet(pos.x, pos.y, pos.z, 1.0f);

    XMMATRIX local = XMMatrixAffineTransformation(S, XMVectorZero(), Q, T);
 
    // 親がいれば親のワールド行列を掛ける（親情報は GameObject 経由）
    XMMATRIX world = local;

    if (gameObject)
    {
        GameObject* parentGO = gameObject->GetParent();
        if (parentGO && parentGO->transform)
        {
            const XMFLOAT4X4& pm = parentGO->transform->GetMatrix();
            XMMATRIX P = XMLoadFloat4x4(&pm);
            world = XMMatrixMultiply(local, P);
        }
    }

    XMStoreFloat4x4(&worldMatrix, world);
}

//-----------------------------
// Dirty 伝播
//-----------------------------
void Transform::MarkDirtyRecursive()
{
    dirty = true;

    if (!gameObject) return;

    const auto& children = gameObject->GetChildren();
    for (auto* c : children)
    {
        if (!c) continue;
        if (c->transform)
        {
            c->transform->MarkDirtyRecursive();
        }
    }
}
