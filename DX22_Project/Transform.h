#pragma once
#include "Vector3.h"
#include "Component.h"

// （必要ならそのまま残してもOK）
constexpr DirectX::XMFLOAT3 Vec3Zero = { 0.0f, 0.0f, 0.0f };
constexpr DirectX::XMFLOAT3 Vec3One = { 1.0f, 1.0f, 1.0f };
constexpr DirectX::XMFLOAT3 Vec3Right = { 1.0f, 0.0f, 0.0f };
constexpr DirectX::XMFLOAT3 Vec3Up = { 0.0f, 1.0f, 0.0f };
constexpr DirectX::XMFLOAT3 Vec3Forward = { 0.0f, 0.0f, 1.0f };

class Transform final : public Component
{
private:
    Vector3  pos = Vector3::zero();
    Vector3  rotate = Vector3::zero();   // (x,y,z) 度数
    Vector3  scale = Vector3::one();

    DirectX::XMFLOAT4X4 worldMatrix{};  // ワールド行列（親子なしならローカル＝ワールド）
    bool dirty = true;

public:
    Transform(Vector3 p = Vector3::zero(),
        Vector3 r = Vector3::zero(),
        Vector3 s = Vector3::one());
    ~Transform() override = default;

    void Start() override {}

    // --- Local TRS ---
    void SetPosition(const Vector3& p) { pos = p;  MarkDirtyRecursive(); }
    void SetRotation(const Vector3& r) { rotate = r;  MarkDirtyRecursive(); }
    void SetScale(const Vector3& s) { scale = s; MarkDirtyRecursive(); }
    void SetTransform(const Vector3& p, const Vector3& r, const Vector3& s)
    {
        pos = p; rotate = r; scale = s; MarkDirtyRecursive();
    }

    const Vector3& GetPosition() const { return pos; }
    const Vector3& GetRotation() const { return rotate; }
    const Vector3& GetScale()    const { return scale; }

    // --- 親子（実際の親子は GameObject が保持する） ---
    // ※ GameObject 側で親子を変更したあと、
    //   keepWorld = true の場合に「ローカルTRSを再計算してワールド位置を維持」したいときに使う。
    void SetParent(Transform* newParent, bool keepWorld = false);

    // --- Convenience ---
    void AddPosition(const Vector3& delta) { pos += delta;  MarkDirtyRecursive(); }
    void AddRotate(const Vector3& delta) { rotate += delta;  MarkDirtyRecursive(); }
    void AddScale(const Vector3& delta) { scale += delta;  MarkDirtyRecursive(); }
    void MulScale(float s) { scale *= s; MarkDirtyRecursive(); }
    DirectX::XMFLOAT3 Forward();
    DirectX::XMFLOAT3 Up();
    DirectX::XMFLOAT3 Right();

    // 回転クォータニオン取得
    DirectX::XMVECTOR GetRotationQuaternionXM() const;   // XMVECTOR
    DirectX::XMFLOAT4 GetRotationQuaternion() const;     // (x,y,z,w)

    // --- Matrix / Position (world) ---
    const DirectX::XMFLOAT4X4& GetMatrix();       // 親込みのワールド行列
    DirectX::XMFLOAT3          GetWorldPosition();// worldMatrix の平行移動成分


private:
    void RebuildIfDirty();
    void MarkDirtyRecursive(); // 自分＋子 GameObject の Transform を dirty に
};
