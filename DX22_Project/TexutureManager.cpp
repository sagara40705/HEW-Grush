#include "TextureManager.h"
#include <cassert>

std::array<std::unique_ptr<Texture>, static_cast<size_t>(TextureTag::Count)> TextureManager::s_textures;

const char* TextureManager::kTexturePath[static_cast<size_t>(TextureTag::Count)] =
{
    /*00*/"Assets/Texture/Title_BackGround.png",
    /*01*/"Assets/Texture/UIGauge.png",
    /*02*/"Assets/Texture/Star.png",
    /*03*/"Assets/Texture/GroundTile.png",
    /*04*/"Assets/Texture/stage.png",
    /*05*/"Assets/Texture/SkyDome.png",
    /*06*/"Assets/Texture/lv1_0_b.png",
    /*07*/"Assets/Texture/lv1_max_b.png",
    /*08*/"Assets/Texture/lv1_0_g.png",
    /*09*/"Assets/Texture/sparklingStar03_ss.png",
    /*10*/"Assets/Texture/ato_kg.png",
    /*11*/"Assets/Texture/count_font_ss.png",
    /*12*/"Assets/Texture/time_colon.png",
    /*13*/"Assets/Texture/time_count_ss.png",
    /*14*/"Assets/Texture/UI_rotation_color.png",
    /*15*/"Assets/Texture/UI_rotation.png",
    /*16*/"Assets/Texture/UI_rotation_moji_color.png",
    /*17*/"Assets/Texture/UI_rotation_moji.png",
    /*18*/"Assets/Texture/UI_rotation_flash.png",
    /*19*/"Assets/Texture/UI_rotation_flash_front.png",
    /*20*/"Assets/Texture/UI_dush_color.png",
    /*21*/"Assets/Texture/UI_dush.png",
    /*22*/"Assets/Texture/UI_dush_moji_color.png",
    /*23*/"Assets/Texture/UI_dush_moji.png",
    /*24*/"Assets/Texture/UI_dush_flash.png",
    /*25*/"Assets/Texture/UI_dush_flash_front.png",
    /*26*/"Assets/Texture/UIFrostNoise08B.png",
    /*27*/"Assets/Texture/effect_lv1_at_long.png",
    /*28*/"Assets/Texture/Glass_Shard.png",
    /*29*/"Assets/Texture/StageMap.png",
    /*30*/"Assets/Texture/option/UI_sigotonimodoru.png",
    /*31*/"Assets/Texture/option/UI_sigotonimodoru_waku.png",
    /*32*/"Assets/Texture/option/UI_sigotonimodoru_sentaku.png",
    /*33*/"Assets/Texture/option/UI_sigotonimodoru_sentakuBreakID.png",
    /*34*/"Assets/Texture/proto.png",
    /*35*/"Assets/Texture/bg_2048_1024.png",
    /*36*/"Assets/Texture/bg_4096_2048.png",
    /*37*/"Assets/Texture/Titele/title_bg.png",
    /*38*/"Assets/Texture/Titele/title_hakaisuru2.png",
    /*39*/"Assets/Texture/Titele/Fade.png",
    /*40*/"Assets/Texture/Titele/fadeinID.png",
    /*41*/"Assets/Texture/Titele/title_Logo.png",
    /*42*/"Assets/Texture/Titele/title_hakaisuru2.png",
    /*43*/"Assets/Texture/fead_in_2_ss.png",
    /*44*/"Assets/Texture/Load_BG.png",
    /*45*/"Assets/Texture/Load_Txt.png",
    /*46*/"Assets/Texture/TasksUI.png",
    /*47*/"Assets/Texture/TutorialLocation.png",
	/*48*/"Assets/Texture/LoadCharacter.png",
};

// 配列のデフォルトパスを使用して登録
bool TextureManager::Load(TextureTag tag)
{
    if (s_textures[static_cast<size_t>(tag)]) return false;

    int index = static_cast<int>(tag);
    const char* filePath = kTexturePath[static_cast<size_t>(tag)];
    assert(index >= 0 && index < static_cast<int>(TextureTag::Count));

    auto tex = std::make_unique<Texture>();
    if (FAILED(tex->Create(filePath)))
        return false;

    s_textures[index] = std::move(tex);
    return true;
}

void TextureManager::Unload(TextureTag tag)
{
    s_textures[static_cast<size_t>(tag)].reset();
}

Texture* TextureManager::Get(TextureTag tag)
{
    return s_textures[static_cast<int>(tag)].get();
}

void TextureManager::Clear()
{
    for (auto& t : s_textures)
    {
        t.reset();
    }
}