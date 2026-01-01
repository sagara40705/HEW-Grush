#include "TextureManager.h"
#include <cassert>

std::array<std::unique_ptr<Texture>, static_cast<size_t>(TextureTag::Count)> TextureManager::s_textures;

const char* TextureManager::kTexturePath[static_cast<size_t>(TextureTag::Count)] =
{
	"Assets/Texture/Title_BackGround.png",  //
	"Assets/Texture/UIGauge.png", 			  // LoadGauge1
	"Assets/Texture/Star.png",   // Star
	"Assets/Texture/lv1_0_b.png",
	"Assets/Texture/lv1_max_b.png", // GaugeMax
	"Assets/Texture/kirakira.png", // GaugeEffect
	"Assets/Texture/ato_kg.png", 	  // ScoreAtoKg
    "Assets/Texture/count_font_ss.png",
	"Assets/Texture/time_colon.png",         //9
	"Assets/Texture/time_count_ss.png",      //10
	"Assets/Texture/UI_rotation_color.png",  //11
	"Assets/Texture/UI_rotation.png",        //12
    "Assets/Texture/UI_dush_color.png",      //13
    "Assets/Texture/UI_dush.png",            //14
    "Assets/Texture/effect_lv1_at_long.png", //15
     "Assets/Texture/UIFrame.png",            //16
    "Assets/Texture/UIGauge.png",            //17
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