// SceneTitle.cpp
#include "SceneSample.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "DirectX.h"
#include "Vector3.h"
using namespace DirectX;

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
        (const EFK_CHAR*)L"Effekseer/Sample/square_b.efkefc"
    );
    if (g_TestEffect.Get() == nullptr) return false;

    g_EfkInitialized = true;
    return true;
}

static void EffekseerFinalizeForSampleScene()
{
    g_TestEffect.Reset();
    g_EfkManager.Reset();
    g_EfkRenderer.Reset();
    g_EfkInitialized = false;
    g_TestHandle = -1;
}

void SceneSample::Load()
{
    // Effekseer 初期化
    if (!InitEffekseerForSampleScene())
    {
        // TODO: ログ出すなど
        return;
    }

    // このシーンに入った瞬間に 1 回だけ再生
    if (g_EfkManager.Get() && g_TestEffect.Get())
    {
        // 原点少し前あたりに出す（座標は適宜調整）
        Effekseer::Vector3D pos(0.0f, -2.0f, 5.0f);
        g_TestHandle = g_EfkManager->Play(g_TestEffect, pos.X, pos.Y, pos.Z);
    }
}

void SceneSample::Unload()
{
    EffekseerFinalizeForSampleScene();
}

void SceneSample::Update()
{
    if (!g_EfkInitialized) return;
    if (g_EfkManager.Get() == nullptr) return;
    constexpr float DT = 1.f / 60.f;

    g_EfkManager->Update(DT);
}

void SceneSample::Draw()
{
    if (!g_EfkInitialized) return;
    if (g_EfkRenderer.Get() == nullptr || g_EfkManager.Get() == nullptr) return;

    // --------------------------------------------------
    // 簡易カメラ行列（サンプル用）
    // 本番では自作 Camera から View / Proj をもらう
    // --------------------------------------------------
    XMVECTOR eye = XMVectorSet(0.0f, 2.0f, -10.0f, 0.0f);
    XMVECTOR target = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    XMMATRIX viewM = XMMatrixLookAtLH(eye, target, up);
    XMMATRIX projM = XMMatrixPerspectiveFovLH(
        XMConvertToRadians(60.0f),
        16.0f / 9.0f,
        0.1f,
        100.0f
    );

    XMFLOAT4X4 viewF, projF;
    XMStoreFloat4x4(&viewF, viewM);
    XMStoreFloat4x4(&projF, projM);

    Effekseer::Matrix44 efkView;
    Effekseer::Matrix44 efkProj;
    memcpy(efkView.Values, &viewF, sizeof(float) * 16);
    memcpy(efkProj.Values, &projF, sizeof(float) * 16);

    g_EfkRenderer->SetCameraMatrix(efkView);
    g_EfkRenderer->SetProjectionMatrix(efkProj);

    // --------------------------------------------------
    // エフェクト描画
    // （ここまでに BackBuffer + DepthStencil は OM にセット済み前提）
    // --------------------------------------------------
    g_EfkRenderer->BeginRendering();
    g_EfkManager->Draw();
    g_EfkRenderer->EndRendering();
	
}

