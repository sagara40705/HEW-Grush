#include "EffekseerSystem.h"
#include "DirectX.h"
#include "Camera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Vector3.h"
using namespace DirectX;

namespace
{
    ::Effekseer::ManagerRef                 g_manager;
    ::EffekseerRendererDX11::RendererRef    g_renderer;
    // サウンドを使うなら
    //::EffekseerSound::SoundRef              g_sound;

    float g_timeSec = 0.0f;

    // DirectXMath の XMFLOAT4X4 → Effekseer::Matrix44 変換
    ::Effekseer::Matrix44 ToEfkMatrix(const XMFLOAT4X4& m)
    {
        ::Effekseer::Matrix44 r;
        r.Values[0][0] = m._11; r.Values[0][1] = m._12; r.Values[0][2] = m._13; r.Values[0][3] = m._14;
        r.Values[1][0] = m._21; r.Values[1][1] = m._22; r.Values[1][2] = m._23; r.Values[1][3] = m._24;
        r.Values[2][0] = m._31; r.Values[2][1] = m._32; r.Values[2][2] = m._33; r.Values[2][3] = m._34;
        r.Values[3][0] = m._41; r.Values[3][1] = m._42; r.Values[3][2] = m._43; r.Values[3][3] = m._44;
        return r;
    }
}

void EffekseerSystem::Init()
{
    // すでに初期化済みなら何もしない
    if (g_manager.Get()) return;

    // 1) Manager 作成（サンプルと同じ 8000）
    g_manager = ::Effekseer::Manager::Create(8000);

    // 2) GraphicsDevice / Renderer 作成
    ID3D11Device* device = GetDevice();
    ID3D11DeviceContext* context = GetContext();

    auto graphics = EffekseerRendererDX11::CreateGraphicsDevice(device, context);
    g_renderer = EffekseerRendererDX11::Renderer::Create(graphics, 8000);

    // 3) 各種 Renderer を Manager に登録
    g_manager->SetSpriteRenderer(g_renderer->CreateSpriteRenderer());
    g_manager->SetRibbonRenderer(g_renderer->CreateRibbonRenderer());
    g_manager->SetRingRenderer(g_renderer->CreateRingRenderer());
    g_manager->SetTrackRenderer(g_renderer->CreateTrackRenderer());
    g_manager->SetModelRenderer(g_renderer->CreateModelRenderer());

    // 4) Loader を Manager に登録
    g_manager->SetTextureLoader(g_renderer->CreateTextureLoader());
    g_manager->SetModelLoader(g_renderer->CreateModelLoader());
    g_manager->SetMaterialLoader(g_renderer->CreateMaterialLoader());
    g_manager->SetCurveLoader(::Effekseer::MakeRefPtr<::Effekseer::CurveLoader>());

    // 5) サウンドを Effekseer 側でも使いたい場合
    /*
    IXAudio2* xa2 = ...;      // XAudio2 をどこかで持っているならここで取得
    g_sound = ::EffekseerSound::Sound::Create(xa2, 16, 16);

    g_manager->SetSoundPlayer(g_sound->CreateSoundPlayer());
    g_manager->SetSoundLoader(g_sound->CreateSoundLoader());
    */

    g_timeSec = 0.0f;
}

void EffekseerSystem::Uninit()
{
    //g_sound.Reset();
    g_renderer.Reset();
    g_manager.Reset();

    g_timeSec = 0.0f;
}

void EffekseerSystem::Update()
{
    if (!g_manager.Get()) return;

    g_timeSec += 1.0f / 60.0f;  // 仮で 60fps 固定。

    ::Effekseer::Manager::UpdateParameter param;
    g_manager->Update(param);
}

void EffekseerSystem::Render3D()
{
    if (!g_manager.Get() || !g_renderer.Get()) return;

    Camera* cam = Camera::Main();
    if (!cam) return;

    // 1) カメラの View / Proj 行列を Effekseer に渡す
    const XMFLOAT4X4& viewDX = cam->GetView();
    const XMFLOAT4X4& projDX = cam->GetProjection();

    ::Effekseer::Matrix44 view = ToEfkMatrix(viewDX);
    ::Effekseer::Matrix44 proj = ToEfkMatrix(projDX);

    g_renderer->SetTime(g_timeSec);
    g_renderer->SetProjectionMatrix(proj);
    g_renderer->SetCameraMatrix(view);

    // 2) ViewerPosition を設定（レイヤーパラメータ）
    ::Effekseer::Manager::LayerParameter layerParam;
    {
        auto eye = cam->gameObject->transform->GetWorldPosition();
        layerParam.ViewerPosition = ::Effekseer::Vector3D(eye.x, eye.y, eye.z);
    }
    g_manager->SetLayerParameter(0, layerParam);

    // 3) BeginRendering → Draw → EndRendering
    g_renderer->BeginRendering();

    ::Effekseer::Manager::DrawParameter drawParam;
    drawParam.ZNear = 0.0f; 
    drawParam.ZFar = 1.0f;
    drawParam.ViewProjectionMatrix = g_renderer->GetCameraProjectionMatrix();

    g_manager->Draw();

    g_renderer->EndRendering();
}

::Effekseer::ManagerRef EffekseerSystem::GetManager()
{
    return g_manager;
}

::EffekseerRendererDX11::RendererRef EffekseerSystem::GetRenderer()
{
    return g_renderer;
}

::Effekseer::EffectRef EffekseerSystem::LoadEffect(const char16_t* path)
{
    if (!g_manager.Get()) return nullptr;
    return ::Effekseer::Effect::Create(g_manager, path);
}

::Effekseer::Handle EffekseerSystem::Play(::Effekseer::EffectRef effect, const Vector3& worldPos)
{
    if (!g_manager.Get() || !effect.Get()) return -1;
    return g_manager->Play(effect, worldPos.x, worldPos.y, worldPos.z);
}

void EffekseerSystem::Stop(::Effekseer::Handle handle)
{
    if (!g_manager.Get()) return;
    g_manager->StopEffect(handle);
}

void EffekseerSystem::SetLocation(::Effekseer::Handle handle, const Vector3& worldPos)
{
    if (!g_manager.Get()) return;
    g_manager->SetLocation(handle, 
        ::Effekseer::Vector3D(worldPos.x, worldPos.y, worldPos.z));
}
