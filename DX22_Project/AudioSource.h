#pragma once
#include "Component.h"
#include "SoundType.h"

//=========================================================
// AudioSource
//   - GameObject にアタッチして SE/BGM を再生するコンポーネント。
//   - 基本は SE 用を想定（BGM は SoundSystem::PlayBGM を推奨）。
//=========================================================
class AudioSource : public MonoBehaviour
{
public:
    //=== 設定パラメータ（Inspector 想定） ===
    SoundCueId    cueId = InvalidSoundCueId;      // 再生するキュー ID
    SoundCategory category = SoundCategory::SE;      // カテゴリ
    bool          playOnAwake = false;                  // Awake 時に自動再生するか
    bool          loop = false;                  // ループ再生
    float         volume = 1.0f;                   // 0.0〜1.0
    float         pitch = 1.0f;                   // 0.5〜2.0 など
    bool          is3D = true;                   // 3D 音として扱うか

private:
    SoundHandle   handle = InvalidSoundHandle;

public:
    AudioSource() = default;
    ~AudioSource() override = default;

    //=== ライフサイクル ===
    void Awake() override;
    void OnDisable() override;
    void OnDestroy() override;
    void Update() override;   // 3D 属性更新用

    //=== API (Unity風) ===
    void Play();
    void Stop();
    bool IsPlaying() const;

    void SetVolume(float v);
    void SetPitch(float p);

    SoundHandle GetHandle() const { return handle; }
};
