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
    /*08*/GaugeFlash,
    /*09*/GaugeEffect,
    /*10*/ScoreAtoKg,
    /*11*/ScoreNumber,
    /*12*/TimerColon,
    /*13*/TimerNumber,
    /*14*/Spin,
    /*15*/SpinMono,
    /*16*/SpinMoji,
    /*17*/SpinMojiMono,
    /*18*/SpinFlash,
    /*19*/SpinFront,
    /*20*/Dash,
    /*21*/DashMono,
    /*22*/DashMoji,
    /*23*/DashMojiMono,
    /*24*/DashFlash,
    /*25*/DashFront,
    /*26*/UIFrostNoise,
    /*27*/EffectLv1,
    /*28*/GlassShard,
    /*29*/StageMap,
    /*30*/UI_font,
    /*31*/UI_Frame,
    /*32*/UI_Break,
    /*33*/UI_BreakID,
    /*34*/Background_Game,
    /*35*/Background_Game2,
    /*36*/Background_Game3,
    /*37*/Title_Main,
    /*38*/Title_Font,
    /*39*/Title_Break,
    /*40*/Title_BreakID,
    /*41*/Title_Loge,
    /*42*/Title_Loge2,
    /*43*/FadeOut,
    /*44*/Loading_BG,
    /*45*/Loading_Txt,
    /*46*/TasksUI,
    /*47*/TutorialLocation,
    /*48*/Loading_Character,
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