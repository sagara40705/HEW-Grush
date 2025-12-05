#pragma once
// === File: CameraShake.h ===
#include "Component.h"
#include "Vector3.h"
#include <cstdint>

enum class ShakeType : std::uint8_t
{
    LEVEL_1 = 0,
    LEVEL_2 = 1,
    LEVEL_3 = 2,
    COUNT = 3,
};
constexpr size_t ToIndex(ShakeType type) noexcept {
    return static_cast<size_t>(type);
}

struct ShakeParam
{
    float duration = 0.f;
    float positionAmplitude = 0.f;
    float rotationAmplitude = 0.f;
    Vector3 positionAxisMask = Vector3::zero();
    Vector3 rotationAxisMask = Vector3::zero();
    float frequency = 0.f;
};

// カメラにアタッチして、Transform を直接揺らすシンプルなコンポーネント
// 他オブジェクトには一切触らない。
class CameraShake final : public MonoBehaviour, public ILateUpdate
{
private:
    ShakeParam param_;
public:
    CameraShake() = default;

    //====================== API ======================
    
    // パラメータで揺れを開始
    void Play(ShakeType type);

    // 強制停止（Transform を元の位置・回転に戻す）
    void Stop();

    bool IsPlaying() const noexcept { return play_; }

    //====================== MonoBehaviour / ILateUpdate ======================

    void OnDisable() override;
    void OnDestroy() override;
    void LateUpdate() override;

private:
    // 内部状態
    bool    play_ = false;
    float   timer_ = 0.0f;
    bool    hasOffset_ = false;

    void ClearOffset_(); // 現フレームのオフセットを Transform から打ち消す
    void StepShake_();   // 1 フレーム分の揺れ更新

    static float RandomRange_(float min, float max);
};
