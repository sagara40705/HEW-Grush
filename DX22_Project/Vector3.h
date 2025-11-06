#pragma once
#include <cmath>
#include <DirectXMath.h>

struct Vector3
{
    float x; float y; float z;

    constexpr Vector3(float _x, float _y, float _z) noexcept : x(_x), y(_y), z(_z) {}
    constexpr Vector3(const DirectX::XMFLOAT3& v) noexcept : x(v.x), y(v.y), z(v.z) {}

    // --- ïœä∑ ---
    operator DirectX::XMFLOAT3() const noexcept { return { x, y, z }; }

    // --- íËêî ---
    static constexpr Vector3 zero()     noexcept { return { 0,0,0 }; }
    static constexpr Vector3 one()      noexcept { return { 1,1,1 }; }
    static constexpr Vector3 up()       noexcept { return { 0,1,0 }; }
    static constexpr Vector3 down()     noexcept { return { 0,-1,0 }; }
    static constexpr Vector3 right()    noexcept { return { 1,0,0 }; }
    static constexpr Vector3 left()     noexcept { return { -1,0,0 }; }
    static constexpr Vector3 forward()  noexcept { return { 0,0,1 }; }
    static constexpr Vector3 back()     noexcept { return { 0,0,-1 }; }
    static constexpr DirectX::XMFLOAT3 XMzero    = { 0,0,0 };
    static constexpr DirectX::XMFLOAT3 XMone     = { 1,1,1 };
    static constexpr DirectX::XMFLOAT3 XMup      = { 0,1,0 };
    static constexpr DirectX::XMFLOAT3 XMforward = { 0,0,1 };

    // --- ââéZéq ---
    Vector3 operator+(const Vector3& v) const noexcept { return { x + v.x, y + v.y, z + v.z }; }
    Vector3 operator-(const Vector3& v) const noexcept { return { x - v.x, y - v.y, z - v.z }; }
    Vector3 operator*(float s) const noexcept { return { x * s, y * s, z * s }; }
    Vector3 operator/(float s) const noexcept { return { x / s, y / s, z / s }; }

    Vector3& operator+=(const Vector3& v) noexcept { x += v.x; y += v.y; z += v.z; return *this; }
    Vector3& operator-=(const Vector3& v) noexcept { x -= v.x; y -= v.y; z -= v.z; return *this; }
    Vector3& operator*=(float s) noexcept { x *= s; y *= s; z *= s; return *this; }

    bool operator==(const Vector3& v) const noexcept { return x == v.x && y == v.y && z == v.z; }
    bool operator!=(const Vector3& v) const noexcept { return !(*this == v); }
    Vector3 operator-() const noexcept { return { -x, -y, -z }; }

    // --- äÓñ{ä÷êî ---
    float Length() const noexcept { return std::sqrt(x * x + y * y + z * z); }
    float SqrLength() const noexcept { return x * x + y * y + z * z; }

    Vector3 Normalized() const noexcept
    {
        float len = Length();
        return (len > 1e-6f) ? (*this / len) : Vector3::zero();
    }
    void Normalize() noexcept
    {
        float len = Length();
        if (len > 1e-6f) { x /= len; y /= len; z /= len; }
    }

    // --- ê√ìIä÷êî ---
    static float Dot(const Vector3& a, const Vector3& b) noexcept
    {
        return a.x * b.x + a.y * b.y + a.z * b.z;
    }
    static Vector3 Cross(const Vector3& a, const Vector3& b) noexcept
    {
        return {
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        };
    }
    static float Distance(const Vector3& a, const Vector3& b) noexcept
    {
        return (a - b).Length();
    }
    static Vector3 Lerp(const Vector3& a, const Vector3& b, float t) noexcept
    {
        return a + (b - a) * t;
    }
};