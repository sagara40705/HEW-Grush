// EffekseerSystem.h
#pragma once

#include <Effekseer.h>
#include <EffekseerRendererDX11.h>
// サウンドも使うなら
//#include <EffekseerSoundXAudio2.h>

#include <d3d11.h>

class Camera;
struct Vector3;

namespace EffekseerSystem
{
    // 初期化／終了（DirectX 初期化の後、解放の前に呼ぶ）
    void Init();
    void Uninit();

    // 毎フレーム
    void Update();      // Manager::Update を呼ぶ
    void Render3D();    // 3D カメラと連携して描画する

    // Manager / Renderer への生アクセス（必要なら）
    ::Effekseer::ManagerRef            GetManager();
    ::EffekseerRendererDX11::RendererRef GetRenderer();

    // 簡易 API：エフェクトロード・再生
    ::Effekseer::EffectRef LoadEffect(const char16_t* path);
    ::Effekseer::Handle    Play(::Effekseer::EffectRef effect, const Vector3& worldPos);
    void                   Stop(::Effekseer::Handle handle);
    void                   SetLocation(::Effekseer::Handle handle, const Vector3& worldPos);
}

