#pragma once
#include <cmath>
#include <DirectXMath.h>

struct Vector2
{
    float x; float y;

    // --- ctor ---
	constexpr Vector2() noexcept : x(0.f), y(0.f) {}
    constexpr Vector2(float _x, float _y) noexcept : x(_x), y(_y) {}
    constexpr Vector2(const DirectX::XMFLOAT2& v) noexcept : x(v.x), y(v.y) {}

    // --- XMFLOAT2 •ÏŠ· ---
    operator DirectX::XMFLOAT2() const noexcept { return { x, y }; }

    // --- static ’è” ---
    static constexpr Vector2 zero()  noexcept { return { 0,0 }; }
    static constexpr Vector2 one()   noexcept { return { 1,1 }; }
    static constexpr Vector2 up()    noexcept { return { 0,1 }; }
    static constexpr Vector2 down()  noexcept { return { 0,-1 }; }
    static constexpr Vector2 right() noexcept { return { 1,0 }; }
    static constexpr Vector2 left()  noexcept { return { -1,0 }; }
	static constexpr Vector2 WindowSize() noexcept { return { 1280.f, 720.f }; }

    static constexpr DirectX::XMFLOAT2 XMzero = { 0,0 };
    static constexpr DirectX::XMFLOAT2 XMone = { 1,1 };

    // --- ‰‰ŽZŽq ---
    Vector2 operator+(const Vector2& v) const noexcept { return { x + v.x, y + v.y }; }
    Vector2 operator-(const Vector2& v) const noexcept { return { x - v.x, y - v.y }; }
    Vector2 operator*(float s) const noexcept { return { x * s, y * s }; }
    Vector2 operator/(float s) const noexcept { return { x / s, y / s }; }
    Vector2 operator*(const Vector2& v) const noexcept { return { x * v.x, y * v.y }; }

    Vector2& operator+=(const Vector2& v) noexcept { x += v.x; y += v.y; return *this; }
    Vector2& operator-=(const Vector2& v) noexcept { x -= v.x; y -= v.y; return *this; }
    Vector2& operator*=(float s) noexcept { x *= s; y *= s; return *this; }

    bool operator==(const Vector2& v) const noexcept { return x == v.x && y == v.y; }
    bool operator!=(const Vector2& v) const noexcept { return !(*this == v); }
    Vector2 operator-() const noexcept { return { -x, -y }; }

    // --- ’·‚³ ---
    float Length() const noexcept { return std::sqrt(x * x + y * y); }
    float SqrLength() const noexcept { return x * x + y * y; }

    Vector2 Normalized() const noexcept
    {
        float len = Length();
        return (len > 1e-6f) ? (*this / len) : Vector2::zero();
    }
    void Normalize() noexcept
    {
        float len = Length();
        if (len > 1e-6f) { x /= len; y /= len; }
    }

    // --- Dot / Distance / Lerp ---
    static float Dot(const Vector2& a, const Vector2& b) noexcept
    {
        return a.x * b.x + a.y * b.y;
    }
    static float Distance(const Vector2& a, const Vector2& b) noexcept
    {
        return (a - b).Length();
    }
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) noexcept
    {
        return a + (b - a) * t;
    }
};
