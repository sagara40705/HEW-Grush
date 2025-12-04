#pragma once
// === File: CameraShake.h ===
#pragma once
#include "Component.h"
#include "Vector3.h"
#include "GameObject.h"
#include <cstdint>
#include <cmath>
#include <cstdlib>

// 画面揺れ管理コンポーネント。
// *** 担当 : 牛田 ***

class CameraShake : public MonoBehaviour
{
public:
    enum class Level : int
    {
        Level_1 = 0,
        Level_2 = 1,
        Level_3 = 2,
        Count = 3,
    };
public:
    CameraShake() = default;

    void Start() override;
    void Update() override;
    void OnDestroy() override;

    // 揺れ開始トリガー（どこからでも呼べる静的関数）
    // amp      : 最大振幅（ワールド座標単位）
    // duration : 継続時間[秒]
    static void StartShake(Level level);

    // Render側から呼ぶ関数。
    // - 引数の GameObject が「揺らす対象かどうか」を内部で判定する。
    // - 揺らすべきならオフセットを返し、揺らさない場合は (0,0,0) を返す。
    static void Shake(GameObject* go, DirectX::XMFLOAT4X4& world);

    // 状態確認（必要なら）
    static bool IsActive();

private:
    // パラメータ種類
    struct Profile
    { 
    float amplitude;  // 最大振幅 
    float duration;   // 継続時間
    float frequency;  // 揺れの速さ
    Vector3 axisMask; // 上下/左右/奥行きの揺れ配分
    };

    // 調整パラメータ
    static constexpr Profile kProfiles_[static_cast<int>(Level::Count)] =
    {
        { 0.15f, 0.15f, 12.0f, Vector3(1.0f, 0.02f, 0.0f) }, // Grade_1
        { 0.30f, 0.25f, 15.0f, Vector3(1.0f, 0.05f, 0.0f) }, // Grade_2
        { 0.45f, 0.35f, 50.0f, Vector3(1.5f, 0.8f, 0.0f) }, // Grade_3
    };

    // シングルトンインスタンス
    static CameraShake* s_instance_;

    // 状態
    static float   s_time_;      // 残り時間
    static float   s_duration_;  // 総時間
    static float   s_amplitude_; // 初期振幅
    static float   s_frequency_; // 現在の揺れ周波数
    static Vector3 s_axisMask_;  // 現在の軸マスク
    static Vector3 s_phase_;     // XYZ の位相
    static Vector3 s_freqMult_;  // 軸ごとの周波数倍率
    static Vector3 s_offset_;    // 今フレームの共通オフセット

private:
    static void UpdateOffset_(float dt);
};
