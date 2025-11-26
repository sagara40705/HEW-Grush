// === File: CameraShake.cpp ===
#include "CameraShake.h"
#include <DirectXMath.h>

using namespace DirectX;

// 静的メンバ定義
CameraShake* CameraShake::s_instance_ = nullptr;
float   CameraShake::s_time_ = 0.0f;
float   CameraShake::s_duration_ = 0.0f;
float   CameraShake::s_amplitude_ = 0.0f;
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

    const std::size_t idx = static_cast<std::size_t>(level);
    if (idx >= std::size(kProfiles_)) return;

    const Profile& p = kProfiles_[idx];
    s_amplitude_ = p.amplitude;
    s_duration_ = (p.duration > 0.0f) ? p.duration : 0.0001f;
    s_time_ = p.duration;
    s_offset_ = Vector3::zero();
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

    // TODO: 実際の deltaTime に差し替え
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
    const float t = s_time_ / s_duration_;
    const float currAmp = s_amplitude_ * t; // 減衰

    auto rand01 = []() -> float {
        return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
        };

    const float rx = rand01() * 2.0f - 1.0f;
    const float ry = rand01() * 2.0f - 1.0f;
    const float rz = rand01() * 2.0f - 1.0f;

    const float len2 = rx * rx + ry * ry + rz * rz;
    if (len2 < 1e-6f)
    {
        s_offset_ = Vector3::zero();
        return;
    }

    const float invLen = 1.0f / std::sqrt(len2);
    s_offset_ = Vector3(
        rx * invLen * currAmp,
        ry * invLen * currAmp,
        rz * invLen * currAmp
    );
}

// Render側から呼ぶ：world に直接オフセットを加算する
void CameraShake::Shake(GameObject* go, XMFLOAT4X4& world)
{
    if (!s_instance_ || s_time_ <= 0.0f || !go)
        return;

    // ここで「揺らす対象かどうか」を全部判定

    if(go->CompareTag(Tag::Player)) return;

    // 必要なら他の除外条件もここに追加
    // if (tag == Tag::UI) return;
    // if (tag == Tag::Weapon) return;

    // 今フレームの共通オフセットを world に加算（Transform はいじらない）
    world._41 += s_offset_.x;
    world._42 += s_offset_.y;
    world._43 += s_offset_.z;
}
