#pragma once
#include <array>
#include <cstdint>
#include "Vector3.h"
#include "Effekseer.h"
#include "EffekseerRendererDX11.h"

// 必要に応じてゲームごとに増やす
enum class EffectID : std::uint8_t
{
    GlassBreak,     // ガラス破壊
    HammerTrail,    // ハンマーの軌跡
    HitSpark,       // ヒットスパーク
    // ...

    Count,
};

class EffekseerSystem final
{
public:
    // --- ライフサイクル ---------------------------------

    // DirectX 初期化後、ゲーム開始時に 1 回呼ぶ
    static void Initialize(int maxInstances = 2000, int maxSprites = 8192);

    // DirectX の Uninit 前に 1 回呼ぶ
    static void Finalize();

    // ゲームの Update 内から呼ぶ（deltaTime は秒）
    static void Update(float deltaTime);

    // ゲームの Draw 内から呼ぶ
    // World の 3D 描画 → EffekseerSystem::Draw3D() → UI 描画、の順を想定
    static void Draw3D();

    // --- API（ゲーム側から使う） -------------------------

    // 事前にエフェクトをロードしておく
    // path は u16 文字列（u"Data/Effect/GlassBreak.efkefc" のような）
    static bool LoadEffect(EffectID id, const char16_t* path);

    // 再生。戻り値の Handle で停止などができる
    static Effekseer::Handle Play(EffectID id, const Vector3& position);

    // 必要ならハンドル操作も提供
    static void Stop(Effekseer::Handle handle);
    static void StopAll(EffectID id);
    static void StopAll();

    // Manager / Renderer を直接触りたいとき用
    static Effekseer::ManagerRef GetManager();
    static EffekseerRendererDX11::RendererRef GetRenderer();

private:
    EffekseerSystem() = default;
    ~EffekseerSystem() = default;

    static EffekseerSystem* s_instance;
    static EffekseerSystem& Instance();

    // Effekseer の中核
    EffekseerRendererDX11::RendererRef renderer_;
    Effekseer::ManagerRef              manager_;
    Effekseer::SettingRef              setting_;
    Effekseer::FileInterfaceRef        fileInterface_;
    Effekseer::TextureLoaderRef        textureLoader_;
    Effekseer::ModelLoaderRef          modelLoader_;
    Effekseer::MaterialLoaderRef       materialLoader_;

    // ゲーム用に保持するエフェクト
    std::array<Effekseer::EffectRef, static_cast<size_t>(EffectID::Count)> effects_{};

    void initializeInternal(int maxInstances, int maxSprites);
    void finalizeInternal();

    // カメラ情報を Effekseer に渡す
    void setupCameraFromMain();

    // DirectX::XMFLOAT4X4 → Effekseer::Matrix44 変換
    static Effekseer::Matrix44 ToEffekseerMatrix(const DirectX::XMFLOAT4X4& m);
};
