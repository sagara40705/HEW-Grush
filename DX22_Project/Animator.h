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

    Model::AnimeNo GetCurrentAnime() const { return currentAnime; }
    bool           IsLoop()          const { return loopFlag; }
    float          GetSpeed()        const { return speed; }
    Model* GetModel() const { return  model; }

private:
    bool playing = false;
    MeshFilter* meshFilter = nullptr;
    MeshRenderer* meshRenderer = nullptr;
    Model* model = nullptr;

    Model::AnimeNo currentAnime = Model::ANIME_NONE;
    bool           loopFlag = true;
    float          speed = 1.0f;
};
