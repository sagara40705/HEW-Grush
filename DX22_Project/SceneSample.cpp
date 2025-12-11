// SceneTitle.cpp
#include "SceneSample.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "DirectX.h"
#include "Vector3.h"
using namespace DirectX;

// Åö Effekseer ópÇÃÉwÉbÉ_í«â¡
#include "EffekseerSystem.h"

::Effekseer::EffectRef testEffect_;

void SceneSample::Load()
{
    auto& world = GetWorld();

    GameObject& mainCamera = world.CreateGameObject();
    mainCamera.AddComponent<Camera>();

    EffekseerSystem::Init();

    testEffect_ = EffekseerSystem::LoadEffect(u"Effekseer/Sample/TriggerLaser.efkefc");
    EffekseerSystem::Play(testEffect_, Vector3(0.f, 0.f, 40.f));


}

void SceneSample::Unload()
{
    testEffect_.Reset();
    EffekseerSystem::Uninit();
}

void SceneSample::Update()
{
    EffekseerSystem::Update();
}

void SceneSample::Draw()
{
    EffekseerSystem::Render3D();
}

