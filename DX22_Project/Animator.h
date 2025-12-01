#pragma once
#include "Component.h"
#include "Model.h"
#include <vector>
#include <string>

// アニメーション再生コンポーネント
class Animator final : public MonoBehaviour
{
public:
    struct Clip
    {
        std::string      name;             // "Idle" "Walk" など
        Model::AnimeNo   animeNo = Model::ANIME_NONE;
        bool             loop = true;
        float            speed = 1.0f;   // クリップ固有の再生速度
    };

private:
    MeshFilter* m_meshFilter = nullptr;
    Model* m_model = nullptr;

    std::vector<Clip> m_clips;
    int   m_currentClip = -1;             // m_clips のインデックス

    float m_time = 0.0f;            // 現在の再生時間 [sec]
    float m_globalSpeed = 1.0f;           // 全体スピード倍率

    // ボーン行列のバッファ (Model::MaxBoneCount 分)
    std::vector<DirectX::XMFLOAT4X4> m_bones;
    bool m_validBones = false;

public:
    Animator() = default;
    ~Animator() override = default;

    void Awake() override;
    void Update() override;

    // クリップを追加（初期化時に登録しておく）
    int  AddClip(const Clip& clip);

    // 名前で再生
    void Play(const std::string& clipName, float startTime = 0.0f);
    void Stop();

    // 状態取得
    bool IsPlaying() const { return m_currentClip >= 0; }
    const Clip* GetCurrentClip() const;

    // MeshRenderer から使う
    const DirectX::XMFLOAT4X4* GetBones() const
    {
        return m_validBones ? m_bones.data() : nullptr;
    }
    uint32_t GetBoneCount() const
    {
        return static_cast<uint32_t>(m_bones.size());
    }
};
