#pragma once
#include <memory>
#include <array>
#include <cstdint>
#include "Texture.h"

enum class TextureTag : uint8_t
{
    /*00*/Title_BG,
    /*01*/LoadGauge,
    /*02*/Star,
    /*03*/GroundTile,
    /*04*/Stage,
    /*05*/SkyDome,
    /*06*/GaugeEmpty,
    /*07*/GaugeMax,
    /*08*/GaugeEffect ,
    /*09*/ScoreAtoKg,
    /*10*/ScoreNumber,
    /*11*/TimerColon,
    /*12*/TimerNumber,
    /*13*/Spin,
    /*14*/SpinMono,
    /*15*/SpinMoji,
    /*16*/SpinMojiMono,
    /*17*/SpinFlash,
    /*18*/SpinFront,
    /*19*/Dash ,
    /*20*/DashMono,
    /*21*/DashMoji,
    /*22*/DashMojiMono,
    /*23*/UIFrostNoise,
    /*24*/EffectLv1,
    /*25*/GlassShard,
    /*26*/StageMap,
    Count,
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