// SceneTitle.cpp
#include "SceneSample.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "DirectX.h"


// ★ Effekseer 用のヘッダ追加
#include <Effekseer.h>
#include <EffekseerRendererDX11.h>

// ★ Effekseer のグローバル（このシーン用の最小構成）
static EffekseerRendererDX11::RendererRef g_EfkRenderer;
static Effekseer::ManagerRef              g_EfkManager;
static Effekseer::EffectRef               g_TestEffect;
static Effekseer::Handle                  g_TestHandle = -1;
static bool                               g_EfkInitialized = false;


static bool InitEffekseerForSampleScene()
{
    if (g_EfkInitialized) return true;

    const int kMaxInstances = 1000;

    g_EfkRenderer = EffekseerRendererDX11::Renderer::Create(
        GetDevice(),
        GetContext(),
        kMaxInstances
    );
    if (g_EfkRenderer.Get() == nullptr) return false;

    g_EfkRenderer->SetRestorationOfStatesFlag(true);

    g_EfkManager = Effekseer::Manager::Create(kMaxInstances);
    if (g_EfkManager.Get() == nullptr) return false;

    g_EfkManager->SetSpriteRenderer(g_EfkRenderer->CreateSpriteRenderer());
    g_EfkManager->SetRibbonRenderer(g_EfkRenderer->CreateRibbonRenderer());
    g_EfkManager->SetRingRenderer(g_EfkRenderer->CreateRingRenderer());
    g_EfkManager->SetTrackRenderer(g_EfkRenderer->CreateTrackRenderer());
    g_EfkManager->SetModelRenderer(g_EfkRenderer->CreateModelRenderer());

    g_EfkManager->SetTextureLoader(g_EfkRenderer->CreateTextureLoader());
    g_EfkManager->SetModelLoader(g_EfkRenderer->CreateModelLoader());
    g_EfkManager->SetMaterialLoader(g_EfkRenderer->CreateMaterialLoader());
    g_EfkManager->SetCurveLoader(Effekseer::MakeRefPtr<Effekseer::CurveLoader>());

    g_TestEffect = Effekseer::Effect::Create(
        g_EfkManager,
        (const EFK_CHAR*)L"Data/Effects/Test.efkefc"
    );
    if (g_TestEffect.Get() == nullptr) return false;

    g_EfkInitialized = true;
    return true;
}


void SceneSample::Load()
{

}

void SceneSample::Unload()
{

}

void SceneSample::Update()
{
    constexpr float DT = 1.f / 60.f;

    g_EfkManager->Update(DT);
}

void SceneSample::Draw()
{
    g_EfkRenderer->BeginRendering(); 

    g_EfkManager->Draw();

    g_EfkRenderer->EndRendering();
	
}

