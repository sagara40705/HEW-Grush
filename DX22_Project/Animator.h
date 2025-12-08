#pragma once
#include "Component.h"
#include "Model.h"

class MeshFilter;
class MeshRenderer;

//=========================================================
// 極小機能版 Animator
//  - Model 内部のアニメをそのまま再生するラッパ
//  - GameObject ごとに 1 つだけ付ける想定
//=========================================================
class Animator final : public MonoBehaviour
{
public:
    Animator() = default;
    ~Animator() override = default;

    void Start() override;
    void Update() override;
    void Play(Model::AnimeNo animeNo, bool loop = true, float speed = 1.0f);

    Model::AnimeNo GetCurrentAnime() const { return currentAnime; }
    bool           IsLoop()          const { return loopFlag; }
    float          GetSpeed()        const { return speed; }

private:
    bool playing = false;
    MeshFilter* meshFilter = nullptr;
    MeshRenderer* meshRenderer = nullptr;
    Model* model = nullptr;

    Model::AnimeNo currentAnime = Model::ANIME_NONE;
    bool           loopFlag = true;
    float          speed = 1.0f;
};
