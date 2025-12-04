// Vector2.h
#pragma once
#include <cmath>
#include <DirectXMath.h>

struct Vector2
{
    float x; float y;

    // --- ctor ---
    constexpr Vector2() noexcept : x(0), y(0) {}
    constexpr Vector2(float _x, float _y) noexcept : x(_x), y(_y) {}
    constexpr Vector2(const DirectX::XMFLOAT2& v) noexcept : x(v.x), y(v.y) {}

    // --- convert ---
    operator DirectX::XMFLOAT2() const noexcept { return { x, y }; }

    // --- consts ---
    static constexpr Vector2 zero()  noexcept { return { 0, 0 }; }
    static constexpr Vector2 one()   noexcept { return { 1, 1 }; }
    static constexpr Vector2 unitX() noexcept { return { 1, 0 }; }
    static constexpr Vector2 unitY() noexcept { return { 0, 1 }; }

    // --- unary ---
    constexpr Vector2 operator+() const noexcept { return *this; }
    constexpr Vector2 operator-() const noexcept { return { -x, -y }; }

    // --- compound ---
    Vector2& operator+=(const Vector2& r) noexcept { x += r.x; y += r.y; return *this; }
    Vector2& operator-=(const Vector2& r) noexcept { x -= r.x; y -= r.y; return *this; }
    Vector2& operator*=(float s)        noexcept { x *= s;   y *= s;   return *this; }
    Vector2& operator/=(float s)        noexcept { x /= s;   y /= s;   return *this; }

    // --- binary ---
    friend Vector2 operator+(Vector2 a, const Vector2& b) noexcept { return a += b; }
    friend Vector2 operator-(Vector2 a, const Vector2& b) noexcept { return a -= b; }
    friend Vector2 operator*(Vector2 a, float s)         noexcept { return a *= s; }
    friend Vector2 operator*(float s, Vector2 a)         noexcept { return a *= s; }
    friend Vector2 operator/(Vector2 a, float s)         noexcept { return a /= s; }

    // （必要なら要素ごとの積）
    static constexpr Vector2 Mul(const Vector2& a, const Vector2& b) noexcept { return { a.x * b.x, a.y * b.y }; }

    // --- length / normalize ---
    float LengthSq() const noexcept { return x * x + y * y; }
    float Length()   const noexcept { return std::sqrt(LengthSq()); }
    Vector2 Normalized() const noexcept {
        float len = Length();
        return (len > 0.0f) ? Vector2{ x / len, y / len } : Vector2::zero();
    }
    void Normalize() noexcept {
        float len = Length();
        if (len > 0.0f) { x /= len; y /= len; }
    }

    // --- math helpers ---
    static constexpr float  Dot(const Vector2& a, const Vector2& b) noexcept { return a.x * b.x + a.y * b.y; }
    // 右法線（+90度回転）UIの境界法線などに
    static constexpr Vector2 Perp(const Vector2& v) noexcept { return { -v.y, v.x }; }

    static Vector2 Clamp(const Vector2& v, const Vector2& minV, const Vector2& maxV) noexcept {
        auto cx = (v.x < minV.x) ? minV.x : (v.x > maxV.x ? maxV.x : v.x);
        auto cy = (v.y < minV.y) ? minV.y : (v.y > maxV.y ? maxV.y : v.y);
        return { cx, cy };
    }

    static constexpr Vector2 Min(const Vector2& a, const Vector2& b) noexcept { return { a.x < b.x ? a.x : b.x, a.y < b.y ? a.y : b.y }; }
    static constexpr Vector2 Max(const Vector2& a, const Vector2& b) noexcept { return { a.x > b.x ? a.x : b.x, a.y > b.y ? a.y : b.y }; }

    static float  Distance(const Vector2& a, const Vector2& b) noexcept { return (a - b).Length(); }
    static Vector2 Lerp(const Vector2& a, const Vector2& b, float t) noexcept { return a + (b - a) * t; }

    // UIでの“ピクセル境界を狙う”用（にじみ防止）
    static Vector2 Round(const Vector2& v) noexcept { return { std::round(v.x), std::round(v.y) }; }
    static Vector2 Floor(const Vector2& v) noexcept { return { std::floor(v.x), std::floor(v.y) }; }
    static Vector2 Ceil(const Vector2& v) noexcept { return { std::ceil(v.x), std::ceil(v.y) }; }

    static bool AlmostEqual(const Vector2& a, const Vector2& b, float eps = 1e-6f) noexcept {
        return std::fabs(a.x - b.x) <= eps && std::fabs(a.y - b.y) <= eps;
    }
};
