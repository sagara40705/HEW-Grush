#pragma once
#include "Component.h"
#include "Model.h"

class MeshFilter;
class MeshRenderer;

class Animator final : public MonoBehaviour
{
public:
    Animator() = default;
    ~Animator() override = default;

    void Start() override;
    void Update() override;
    void Play(Model::AnimeNo animeNo, bool loop = true, float speed = 1.0f);
    void Stop(); 
    void SetTimeSeconds(float time);
    float GetCurrentAnimeTime() const;


    Model::AnimeNo GetCurrentAnime() const { return currentAnime_; }
    bool           IsLoop()          const { return loopFlag; }
    float          GetSpeed()        const { return speed; }
    Model* GetModel() const { return  model_; }

private:
    bool isPlaying_ = false;
    MeshFilter* meshFilter = nullptr;
    MeshRenderer* meshRenderer = nullptr;
    Model* model_ = nullptr;

    Model::AnimeNo currentAnime_ = Model::ANIME_NONE;
    bool           loopFlag = true;
    float          speed = 1.0f;
};
