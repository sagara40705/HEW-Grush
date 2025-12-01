// === File: CameraShake.cpp ===
#include "CameraShake.h"
#include "GameObject.h"
#include <DirectXMath.h>
#include "DebugLog.h"

using namespace DirectX;

// 静的メンバ定義
CameraShake* CameraShake::s_instance_ = nullptr;
float   CameraShake::s_time_ = 0.0f;
float   CameraShake::s_duration_ = 0.0f;
float   CameraShake::s_amplitude_ = 0.0f;
float   CameraShake::s_frequency_ = 0.0f;
Vector3 CameraShake::s_axisMask_ = Vector3(1, 1, 0);
Vector3 CameraShake::s_phase_ = Vector3::zero();
Vector3 CameraShake::s_freqMult_ = Vector3(1.0f, 1.35f, 1.75f);
Vector3 CameraShake::s_offset_ = Vector3::zero();

// プロファイル配列定義
constexpr CameraShake::Profile CameraShake::kProfiles_[];

void CameraShake::Start()
{
    // 最初にアタッチされた1個だけを有効インスタンスにする
    s_instance_ = this;
}

void CameraShake::OnDestroy()
{
    if (s_instance_ == this)
    {
        s_instance_ = nullptr;
        s_time_ = 0.0f;
        s_duration_ = 0.0f;
        s_amplitude_ = 0.0f;
        s_offset_ = Vector3::zero();
    }
}

void CameraShake::StartShake(Level level)
{
    if (!s_instance_) return;

    int idx = static_cast<int>(level);
    if (idx >= std::size(kProfiles_)) return;

    const Profile& p = kProfiles_[idx];
    s_amplitude_ = p.amplitude;
    s_duration_ = (p.duration > 0.0f) ? p.duration : 0.0001f;
    s_time_ = p.duration;
    s_frequency_ = p.frequency;
    s_axisMask_ = p.axisMask;
    s_phase_ = Vector3(0.f, 1.0472f, 2.0944f);
    s_offset_ = Vector3::zero();

    Debug::Log("CameraShake::StartShake");
}

bool CameraShake::IsActive()
{
    return (s_time_ > 0.0f);
}

void CameraShake::Update()
{
    if (s_time_ <= 0.0f)
    {
        s_offset_ = Vector3::zero();
        return;
    }

    // 将来的に実際の deltaTime に差し替え
    constexpr float DT = 1.0f / 60.0f;
    UpdateOffset_(DT);
}

void CameraShake::UpdateOffset_(float dt)
{
    s_time_ -= dt;
    if (s_time_ <= 0.0f)
    {
        s_time_ = 0.0f;
        s_offset_ = Vector3::zero();
        return;
    }

    // 0〜1（残り時間比）
    const float tRem = s_time_ / s_duration_;
    const float damping = tRem * tRem;          // Ease-out 的な減衰
    const float amp = s_amplitude_ * damping;

    if (amp <= 0.0f || s_frequency_ <= 0.0f)
    {
        s_offset_ = Vector3::zero();
        return;
    }

    // ---- 位相更新 ----
    // frequency[Hz] を rad/秒 に変換して位相加算
    constexpr float TWO_PI = 6.28318530718f;
    const float omega = s_frequency_ * TWO_PI;  // 角速度[rad/sec]

    s_phase_ += (s_freqMult_ * (omega * dt));

    // 位相のラップ（オーバーフロー防止）
    auto WrapPhase = [](float& a)
        {
            if (a > TWO_PI)      a -= TWO_PI;
            else if (a < -TWO_PI) a += TWO_PI;
        };
    WrapPhase(s_phase_.x);
    WrapPhase(s_phase_.y);
    WrapPhase(s_phase_.z);

    // ---- 軸ごとの揺れ生成 ----
    const float x = std::sin(s_phase_.x) * s_axisMask_.x;
    const float y = std::sin(s_phase_.y) * s_axisMask_.y;
    const float z = std::sin(s_phase_.z) * s_axisMask_.z;

    // 最終オフセット
    s_offset_.x = x * amp;
    s_offset_.y = y * amp;
    s_offset_.z = z * amp;
}

// Render側から呼ぶ：world に直接オフセットを加算する
void CameraShake::Shake(GameObject* go, XMFLOAT4X4& world)
{
    if (!s_instance_ || s_time_ <= 0.0f || !go)
        return;

    // 揺らす対象かどうかを全部判定
    if(go->CompareTag(Tag::Player)) return;

    // 今フレームの共通オフセットを world matrix に加算
    world._41 += s_offset_.x;
    world._42 += s_offset_.y;
    world._43 += s_offset_.z;
}
