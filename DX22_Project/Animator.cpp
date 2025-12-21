#include "Animator.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "ShaderList.h"
#include "DebugLog.h"
#include <cassert>

//---------------------------------------------------------
// Start : 参照取得など
//---------------------------------------------------------
void Animator::Start()
{
    if (!gameObject) return;

    meshFilter = gameObject->GetComponent<MeshFilter>();
    meshRenderer = gameObject->GetComponent<MeshRenderer>();
    assert(meshFilter && meshRenderer && "Animator::Start()内の参照がnullptrです");
   
    if (meshFilter)
    {
        model_ = meshFilter->GetModel();
        assert(model_ && "Animator, meshFilteのmodelがnullptrです");

   /*     model_ = ModelManager::Get(ModelTag::Player);
        currentAnime = model_->AddAnimation("Assets/Model/animation_hashiru.fbx");
        Play(currentAnime, false, 1.0f);*/
        DebugLog::Output("Start Player Animation");
    }

    // アニメーション付きモデル用 VS を使うようにしておく
    if (meshRenderer)
    {
        meshRenderer->vsKind = ShaderList::VS_ANIME;
    }
}

//---------------------------------------------------------
// Play : アニメーション再生
//---------------------------------------------------------
void Animator::Play(Model::AnimeNo animeNo, bool loop, float spd)
{
    if (!model_) return;
    if (animeNo == Model::ANIME_NONE) return;
    isPlaying_ = true;

    currentAnime_ = animeNo;
    loopFlag = loop;
    speed = spd;

    // Model 側に再生を指示
    model_->PlayAnime(animeNo, loop, speed);
}

//---------------------------------------------------------
// Stop : アニメーション停止
//---------------------------------------------------------
void Animator::Stop()
{
    if (!model_) return;

    model_->StopAnime();
    isPlaying_ = false;
    currentAnime_ = Model::ANIME_NONE;
}

//---------------------------------------------------------
// SetTimeSeconds: 指定秒数までアニメーションをスキップ
//---------------------------------------------------------
void Animator::SetTimeSeconds(float time)
{
    if (!model_) return;
    if (currentAnime_ == Model::ANIME_NONE) return;

    model_->SetAnimeTime(currentAnime_, time);
}

//---------------------------------------------------------
// return: 現在のアニメーションの秒数
//---------------------------------------------------------
float Animator::GetCurrentAnimeTime() const
{
    if (!model_) return 0.0f;

    const auto* info = model_->GetPlayAnimeInfo();
    return info ? info->nowTime : 0.0f;
}

//---------------------------------------------------------
// Update : 毎フレームアニメ時間を進める
//---------------------------------------------------------
void Animator::Update()
{
    if (!model_ || !isPlaying_) return;
    if (currentAnime_ == Model::ANIME_NONE) return;

    constexpr float DT = 1.0f / 60.0f;

    model_->Step(DT);
}
