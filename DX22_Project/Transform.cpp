#include "Transform.h"
using namespace DirectX;

static inline XMVECTOR LoadF3(const XMFLOAT3& v) { return XMLoadFloat3(&v); }
static inline XMVECTOR LoadF4(const XMFLOAT4& v) { return XMLoadFloat4(&v); }

void Transform::SetParent(Transform* newParent, bool keepWorld)
{
    if (newParent == parent) return;

    // 既存親から外す
    if (parent) parent->DetachChild(this);

    // 現在のワールドを保持
    XMMATRIX curWorld = XMLoadFloat4x4(&GetMatrix());

    parent = newParent;
    if (parent) parent->AttachChild(this);

    if (keepWorld)
    {
        // 目標：Local = World * inverse(ParentWorld)
        XMMATRIX P = parent ? XMLoadFloat4x4(&parent->GetMatrix()) : XMMatrixIdentity();
        XMVECTOR det{};
        XMMATRIX Pinv = XMMatrixInverse(&det, P);
        XMMATRIX L = XMMatrixMultiply(curWorld, Pinv);

        // 分解（S,Q,T）
        XMVECTOR S, RQ, T;
        if (XMMatrixDecompose(&S, &RQ, &T, L))
        {
            XMFLOAT3 pf{}, sf{}; XMFLOAT4 rqf{};
            XMStoreFloat3(&pf, T);
            XMStoreFloat3(&sf, S);
            XMStoreFloat4(&rqf, RQ);

            // クォータニオン→オイラー（X→Y→Z）
            // 直接度数に戻す
            // NOTE: XMQuaternionRotationRollPitchYaw 逆変換関数は無いので行列→オイラーで近似
            XMMATRIX Rm = XMMatrixRotationQuaternion(RQ);
            // YawPitchRoll抽出（簡易）
            float pitch, yaw, roll;
            {
                // Row-major 前提
                float r20 = Rm.r[2].m128_f32[0];
                float r21 = Rm.r[2].m128_f32[1];
                float r22 = Rm.r[2].m128_f32[2];

                yaw = std::atan2(Rm.r[0].m128_f32[2], Rm.r[2].m128_f32[2]); // 近似
                pitch = std::asin(-r21);
                roll = std::atan2(Rm.r[1].m128_f32[0], Rm.r[1].m128_f32[1]);
            }

            pos = XMFLOAT3(pf.x, pf.y, pf.z);
            scale = XMFLOAT3(sf.x, sf.y, sf.z);
            rotate = XMFLOAT3(XMConvertToDegrees(pitch), XMConvertToDegrees(yaw), XMConvertToDegrees(roll));
        }
        // 失敗時はそのまま（TODO: 必要なら安全値）
    }

    MarkDirtyRecursive();
}

DirectX::XMFLOAT3 Transform::Forward()
{
    RebuildIfDirty();
    XMFLOAT3 f{ worldMatrix._31, worldMatrix._32, worldMatrix._33 };
    // 正規化
    XMVECTOR v = XMVector3Normalize(LoadF3(f)); XMStoreFloat3(&f, v); return f;
}

DirectX::XMFLOAT3 Transform::Up()
{
    RebuildIfDirty();
    XMFLOAT3 u{ worldMatrix._21, worldMatrix._22, worldMatrix._23 };
    XMVECTOR v = XMVector3Normalize(LoadF3(u)); XMStoreFloat3(&u, v); return u;
}

DirectX::XMFLOAT3 Transform::Right()
{
    RebuildIfDirty();
    XMFLOAT3 r{ worldMatrix._11, worldMatrix._12, worldMatrix._13 };
    XMVECTOR v = XMVector3Normalize(LoadF3(r)); XMStoreFloat3(&r, v); return r;
}

// 度→クォータニオン化（X→Y→Z 順：pitch, yaw, roll）
XMVECTOR Transform::GetRotationQuaternionXM() const 
{
    const float pitch = XMConvertToRadians(rotate.x);
    const float yaw = XMConvertToRadians(rotate.y);
    const float roll = XMConvertToRadians(rotate.z);
    return XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
}
XMFLOAT4 Transform::GetRotationQuaternion() const
{
    XMFLOAT4 out{};
    XMStoreFloat4(&out, GetRotationQuaternionXM());
    return out;
}

const XMFLOAT4X4& Transform::GetMatrix() 
{
    RebuildIfDirty();
    return worldMatrix;
}

XMFLOAT3 Transform::GetWorldPosition() 
{
    // 行列がDirtyならここで再計算
    RebuildIfDirty();
    // worldMatrix の平行移動成分
    return XMFLOAT3(worldMatrix._41, worldMatrix._42, worldMatrix._43);
}

void Transform::RebuildIfDirty()
{
    if (!dirty) return;
    dirty = false;

    const XMVECTOR S = LoadF3(scale);
    const XMVECTOR Q = GetRotationQuaternionXM();
    const XMVECTOR T = LoadF3(pos) + XMVectorSet(0, 0, 0, 1);

    XMMATRIX L = XMMatrixAffineTransformation(S, XMVectorZero(), Q, LoadF3(pos));

    // Parent を掛ける（row-major 想定：Local * ParentWorld）
    if (parent)
    {
        const XMMATRIX P = XMLoadFloat4x4(&parent->GetMatrix());
        L = XMMatrixMultiply(L, P);
    }

    //const XMMATRIX M = XMMatrixAffineTransformation(S, XMVectorZero(), Q, T);
    XMStoreFloat4x4(&worldMatrix, L);
}

void Transform::MarkDirtyRecursive()
{
    // 自分は dirty に
    dirty = true;
    // 子に伝播
    for (Transform* c : children)
    {
        if (!c) continue;
        c->MarkDirtyRecursive();
    }
}

void Transform::AttachChild(Transform* c)
{
    if (!c) return;
    children.push_back(c);
}

void Transform::DetachChild(Transform* c)
{
    if (!c) return;
    for (size_t i = 0; i < children.size(); ++i)
    {
        if (children[i] == c) { children.erase(children.begin() + i); break; }
    }
}
