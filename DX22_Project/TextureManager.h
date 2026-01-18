#pragma once
#include <memory>
#include <array>
#include <cstdint>
#include "Texture.h"

enum class TextureTag : uint8_t
{
    Title_BG = 0,
    LoadGauge = 1,
    Star = 2,
    GroundTile = 3,
    Stage = 4,
    SkyDome = 5,
    GaugeEmpty = 6,
    GaugeMax = 7,
    GaugeEffect = 8,
    ScoreAtoKg = 9,
    ScoreNumber = 10,
    TimerColon = 11,
    TimerNumber = 12,
    Spin = 13,
    SpinMono = 14,
    SpinMoji = 15,
    SpinMojiMono = 16,
    SpinFlash = 17,
    SpinFront = 18,
    Dash = 19,
    DashMono = 20,
    DashMoji = 21,
    DashMojiMono = 22,
	UIFrostNoise = 23,
	EffectLv1 = 24,
	GlassShard = 25,
    TasksUI,
	TutorialLocation,
    Count
};

class TextureManager
{
private:
    static std::array<std::unique_ptr<Texture>, static_cast<size_t>(TextureTag::Count)> s_textures;
    static const char* kTexturePath[static_cast<size_t>(TextureTag::Count)];

public:
    /// 成功時 true, 失敗時 false
    static bool Load(TextureTag tag);   // ファイルパスを自動参照
    static void Unload(TextureTag tag);

    /// 取得：tag に対応する Texture のポインタを返す（なければ nullptr）
    static Texture* Get(TextureTag tag);

    /// 全削除：登録されているすべての Texture を解放
    static void Clear();
};