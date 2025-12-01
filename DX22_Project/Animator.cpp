#include "Animator.h"
#include "MeshFilter.h"
#include "GameObject.h"
// Time クラスなどがあるならインクルード
// #include "Time.h"

using namespace DirectX;

void Animator::Awake()
{
    m_meshFilter = gameObject->GetComponent<MeshFilter>();
    if (m_meshFilter)
    {
        m_model = m_meshFilter->GetModel();
    }

    if (m_model)
    {
        m_bones.resize(Model::MaxBoneCount());
    }
}

int Animator::AddClip(const Clip& clip)
{
    m_clips.push_back(clip);
    return static_cast<int>(m_clips.size() - 1);
}

const Animator::Clip* Animator::GetCurrentClip() const
{
    if (m_currentClip < 0 || m_currentClip >= static_cast<int>(m_clips.size()))
        return nullptr;
    return &m_clips[m_currentClip];
}

void Animator::Play(const std::string& clipName, float startTime)
{
    if (!m_model) return;

    for (int i = 0; i < static_cast<int>(m_clips.size()); ++i)
    {
        if (m_clips[i].name == clipName)
        {
            m_currentClip = i;
            m_time = startTime;
            m_validBones = false;
            return;
        }
    }
}

void Animator::Stop()
{
    m_currentClip = -1;
    m_validBones = false;
}

void Animator::Update()
{
    if (!m_model || m_currentClip < 0) return;

    Clip& clip = m_clips[m_currentClip];
    if (clip.animeNo == Model::ANIME_NONE) return;

    const Model::Animation* anim = m_model->GetAnimation(clip.animeNo);
    if (!anim) return;

    // ---- Δt を取得（エンジン側に合わせて変更）----
    float dt = 1.0f / 60.0f;
    // 例: Time クラスがあるならこうする
    // float dt = Time::DeltaTime();

    // ---- 時間更新（Animator 側でループ処理）----
    m_time += dt * clip.speed * m_globalSpeed;

    if (clip.loop && anim->totalTime > 0.0f)
    {
        while (m_time >= anim->totalTime)
        {
            m_time -= anim->totalTime;
        }
    }
    else
    {
        if (m_time > anim->totalTime)
        {
            m_time = anim->totalTime;
            // ループしない場合、ここで Stop() しても良い
        }
    }

    // ---- Model に評価させて bones に書き込む ----
    m_model->SampleAnimationToBones(
        clip.animeNo,
        m_time,
        m_bones.data(),
        static_cast<uint32_t>(m_bones.size())
    );

    m_validBones = true;
}
