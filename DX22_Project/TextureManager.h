#pragma once
#include <memory>
#include <array>
#include <cstdint>
#include "Texture.h"

enum class TextureTag : uint8_t
{
    Title_BG,
    LoadGauge,
    Star,
    GaugeEmpty,
    GaugeMax,
    GaugeEffect,
    ScoreAtoKg,
    ScoreNumber,
    TimerColon,
    TimerNumber,
<<<<<<< Updated upstream
    Spin,
    SpinMono,
    Dash,
    DashMono,
    Count
=======
    Count,
    Result
>>>>>>> Stashed changes
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