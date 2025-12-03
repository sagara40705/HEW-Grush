#include "Animator.h"
#include "MeshFilter.h"
#include "MeshRenderer.h"
#include "GameObject.h"
#include "ShaderList.h"

//---------------------------------------------------------
// Awake : 参照取得など
//---------------------------------------------------------
void Animator::Start()
{
    if (!gameObject) return;

    meshFilter = gameObject->GetComponent<MeshFilter>();
    meshRenderer = gameObject->GetComponent<MeshRenderer>();

    if (meshFilter)
    {
        //model = meshFilter->GetModel();
        //currentAnime = model->AddAnimation("Assets/Model/playerchara_20251202/space_suit_12.fbx");
        //model->Play(currentAnime, true, 1.0f);
    }

    // アニメーション付きモデル用 VS を使うようにしておく
    if (meshRenderer)
    {
        //meshRenderer->vsKind = ShaderList::VS_ANIME;
    }
}

//---------------------------------------------------------
// Play : 単純再生
//---------------------------------------------------------
void Animator::Play(Model::AnimeNo animeNo, bool loop, float spd)
{
    if (!model) return;
    if (animeNo == Model::ANIME_NONE) return;

    currentAnime = animeNo;
    loopFlag = loop;
    speed = spd;

    // Model 側に再生を指示
    model->Play(animeNo, loop, speed);
}

//---------------------------------------------------------
// Update : 毎フレームアニメ時間を進める
//---------------------------------------------------------
void Animator::Update()
{
    if (!model) return;
    if (currentAnime == Model::ANIME_NONE) return;

    constexpr float DT = 1.0f / 60.0f;

    model->Step(DT);
}
