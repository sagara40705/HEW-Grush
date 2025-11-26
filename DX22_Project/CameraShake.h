#pragma once
// === File: CameraShake.h ===
#pragma once
#include "Component.h"
#include "Vector3.h"
#include "GameObject.h"
#include "Layer.h"     // Tag::Player など
#include <cstdint>
#include <cmath>
#include <cstdlib>

// 画面揺れ管理コンポーネント。
// *** 担当 : 牛田 ***

class CameraShake : public MonoBehaviour
{
public:
    enum class Level : std::uint8_t
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
    static void Shake(GameObject* go, XMFLOAT4X4& world);

    // 状態確認（必要なら）
    static bool IsActive();

private:
    // プロファイル1つ分
    struct Profile{ 
    float amplitude; // 最大振幅 
    float duration;  // 継続時間
    };
    // レベルごとのパラメータ
    static constexpr Profile kProfiles_[] ={
        { 0.03f, 0.15f }, // Small
        { 0.10f, 0.25f }, // Medium
        { 0.25f, 0.35f }, // Large
    };

    // シングルトン的に1インスタンスだけ生かす
    static CameraShake* s_instance_;

    // 状態
    static float   s_time_;      // 残り時間
    static float   s_duration_;  // 総時間
    static float   s_amplitude_; // 初期振幅
    static Vector3 s_offset_;    // 今フレームの共通オフセット

private:
    static void UpdateOffset_(float dt);
};
