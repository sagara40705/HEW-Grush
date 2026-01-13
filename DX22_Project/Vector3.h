#pragma once
#include <cmath>
#include <DirectXMath.h>

// UnityEngine.Vector3 をかなり近い感覚で使える C++ 実装
struct Vector3
{
    float x; float y; float z;

    //=====================
    // コンストラクタ
    //=====================
    constexpr Vector3() noexcept : x(0.0f), y(0.0f), z(0.0f) {}

    constexpr Vector3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}

    constexpr Vector3(const DirectX::XMFLOAT3& v) noexcept : x(v.x), y(v.y), z(v.z) {}

    //=====================
    // DirectXMath 連携
    //=====================
    // XMFLOAT3 への暗黙変換
    operator DirectX::XMFLOAT3() const noexcept { return { x, y, z }; }

    // XMVECTORへ変換
    DirectX::XMVECTOR ToXM() const noexcept { return DirectX::XMVectorSet(x, y, z, 0.0f); }

    // XMVECTOR から Vector3 を作成
    static Vector3 FromXM(DirectX::FXMVECTOR v) noexcept
    {
        DirectX::XMFLOAT3 tmp;
        DirectX::XMStoreFloat3(&tmp, v);
        return { tmp.x, tmp.y, tmp.z };
    }

    //=====================
    // 定数ベクトル (Unity風)
    //=====================
    // Unity: 
    static constexpr Vector3 zero()     noexcept { return { 0.0f,  0.0f,  0.0f }; }
    static constexpr Vector3 one()      noexcept { return { 1.0f,  1.0f,  1.0f }; }
    static constexpr Vector3 up()       noexcept { return { 0.0f,  1.0f,  0.0f }; }
    static constexpr Vector3 down()     noexcept { return { 0.0f, -1.0f,  0.0f }; }
    static constexpr Vector3 right()    noexcept { return { 1.0f,  0.0f,  0.0f }; }
    static constexpr Vector3 left()     noexcept { return { -1.0f, 0.0f,  0.0f }; }
    static constexpr Vector3 forward()  noexcept { return { 0.0f,  0.0f,  1.0f }; }
    static constexpr Vector3 back()     noexcept { return { 0.0f,  0.0f, -1.0f }; }

    // DirectX 用の定数（必要なら使用）
    static constexpr DirectX::XMFLOAT3 XMzero{ 0.0f, 0.0f, 0.0f };
    static constexpr DirectX::XMFLOAT3 XMone{ 1.0f, 1.0f, 1.0f };
    static constexpr DirectX::XMFLOAT3 XMup{ 0.0f, 1.0f, 0.0f };
    static constexpr DirectX::XMFLOAT3 XMforward{ 0.0f, 0.0f, 1.0f };

    //=====================
    // 演算子 (Unity 互換志向)
    //=====================
    // Vector + Vector
    Vector3 operator+(const Vector3& v) const noexcept
    {
        return { x + v.x, y + v.y, z + v.z };
    }
    // Vector - Vector

    Vector3 operator-(const Vector3& v) const noexcept
    {
        return { x - v.x, y - v.y, z - v.z };
    }

    // 各成分乗算 (Unity: a * b)
    Vector3 operator*(const Vector3& v) const noexcept
    {
        return { x * v.x, y * v.y, z * v.z };
    }

    // 各成分除算 (Unity: a / b)
    Vector3 operator/(const Vector3& v) const noexcept
    {
        return { x / v.x, y / v.y, z / v.z };
    }

    // スカラー乗算 (v * s)
    Vector3 operator*(float s) const noexcept
    {
        return { x * s, y * s, z * s };
    }

    // スカラー除算 (v / s)
    Vector3 operator/(float s) const noexcept
    {
        return { x / s, y / s, z / s };
    }

    // 左からのスカラー乗算 (s * v)
    friend Vector3 operator*(float s, const Vector3& v) noexcept
    {
        return { v.x * s, v.y * s, v.z * s };
    }

    Vector3& operator+=(const Vector3& v) noexcept
    {
        x += v.x; y += v.y; z += v.z;
        return *this;
    }

    Vector3& operator-=(const Vector3& v) noexcept
    {
        x -= v.x; y -= v.y; z -= v.z;
        return *this;
    }

    Vector3& operator*=(float s) noexcept
    {
        x *= s; y *= s; z *= s;
        return *this;
    }

    Vector3& operator/=(float s) noexcept
    {
        x /= s; y /= s; z /= s;
        return *this;
    }

    bool operator==(const Vector3& v) const noexcept
    {
        return x == v.x && y == v.y && z == v.z;
    }

    bool operator!=(const Vector3& v) const noexcept
    {
        return !(*this == v);
    }

    Vector3 operator-() const noexcept
    {
        return { -x, -y, -z };
    }

    //=====================
    // 長さ関連 (Unity: magnitude, sqrMagnitude)
    //=====================

    float magnitude() const noexcept
    {
        return std::sqrt(x * x + y * y + z * z);
    }

    float sqrMagnitude() const noexcept
    {
        return x * x + y * y + z * z;
    }

    // 互換用エイリアス
    float Length() const noexcept { return magnitude(); }
    float SqrLength() const noexcept { return sqrMagnitude(); }

    Vector3 normalized() const noexcept
    {
        float len = magnitude();
        return (len > 1e-6f) ? (*this / len) : Vector3::zero();
    }

    // Unity と同名版
    Vector3 Normalized() const noexcept { return normalized(); }

    void Normalize() noexcept
    {
        float len = magnitude();
        if (len > 1e-6f)
        {
            x /= len; y /= len; z /= len;
        }
    }

    //=====================
    // 静的ユーティリティ (Unity 互換)
    //=====================

    // Dot(a,b)
    static float Dot(const Vector3& a, const Vector3& b) noexcept
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }

    // Cross(a,b)
    static Vector3 Cross(const Vector3& a, const Vector3& b) noexcept
    {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }

    // Distance(a,b)
    static float Distance(const Vector3& a, const Vector3& b) noexcept
    {
        return (a - b).magnitude();
    }

    // Lerp(a,b,t) [t は 0～1 を想定]
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) noexcept
    {
        if (t < 0.0f) t = 0.0f;
        if (t > 1.0f) t = 1.0f;
        return a + (b - a) * t;
    }

    // LerpUnclamped(a,b,t)
    static Vector3 LerpUnclamped(const Vector3& a, const Vector3& b, float t) noexcept
    {
        return a + (b - a) * t;
    }

    // Scale(a,b) 各成分乗算 (Unity: Vector3.Scale)
    static Vector3 Scale(const Vector3& a, const Vector3& b) noexcept
    {
        return { a.x * b.x, a.y * b.y, a.z * b.z };
    }

    // Angle(from, to) 2ベクトルのなす角 (度数)
    static float Angle(const Vector3& from, const Vector3& to) noexcept
    {
        float denom = std::sqrt(from.sqrMagnitude() * to.sqrMagnitude());
        if (denom < 1e-6f) return 0.0f;

        float d = Dot(from, to) / denom;
        if (d < -1.0f) d = -1.0f;
        if (d > 1.0f) d = 1.0f;

        float rad = std::acos(d);
        return DirectX::XMConvertToDegrees(rad);
    }

    // ClampMagnitude(v, maxLength)
    static Vector3 ClampMagnitude(const Vector3& v, float maxLength) noexcept
    {
        float sqLen = v.sqrMagnitude();
        float maxSq = maxLength * maxLength;
        if (sqLen > maxSq && sqLen > 1e-6f)
        {
            float len = std::sqrt(sqLen);
            float scale = maxLength / len;
            return v * scale;
        }
        return v;
    }

    // Project(v, onNormal) : v を onNormal 方向へ射影
    static Vector3 Project(const Vector3& v, const Vector3& onNormal) noexcept
    {
        float denom = onNormal.sqrMagnitude();
        if (denom < 1e-6f) return Vector3::zero();
        float d = Dot(v, onNormal) / denom;
        return onNormal * d;
    }

    // Reflect(dir, normal) : 法線 normal による反射ベクトル
    static Vector3 Reflect(const Vector3& dir, const Vector3& normal) noexcept
    {
        // Unity と同様: r = d - 2 * dot(d,n) * n
        float d = Dot(dir, normal);
        return dir - 2.0f * d * normal;
    }

    // MoveTowards(current, target, maxDistanceDelta)
    static Vector3 MoveTowards(const Vector3& current,
        const Vector3& target,
        float maxDistanceDelta) noexcept
    {
        Vector3 delta = target - current;
        float dist = delta.magnitude();
        if (dist <= maxDistanceDelta || dist < 1e-6f)
        {
            return target;
        }
        return current + delta * (maxDistanceDelta / dist);
    }
};
