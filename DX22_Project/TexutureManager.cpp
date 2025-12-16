#include "TextureManager.h"
#include <cassert>

std::array<std::unique_ptr<Texture>, static_cast<size_t>(TextureTag::Count)> TextureManager::s_textures;

const char* TextureManager::kTexturePath[static_cast<size_t>(TextureTag::Count)] =
{
    "Assets/Texture/Title_BackGround.png",
    "Assets/Texture/UIGauge.png",
    "Assets/Texture/Star.png",
    "Assets/Texture/GroundTile.png",
    "Assets/Texture/stage.png",
    "Assets/Texture/SkyDome.png",
    "Assets/Texture/lv1_0_b.png",
    "Assets/Texture/lv1_max_b.png",
    "Assets/Texture/sparklingStar03_ss.png",
    "Assets/Texture/ato_kg.png",
    "Assets/Texture/count_font_ss.png",
    "Assets/Texture/time_colon.png",
    "Assets/Texture/time_count_ss.png",
    "Assets/Texture/UI_rotation_color.png",
    "Assets/Texture/UI_rotation.png",
    "Assets/Texture/UI_rotation_moji_color.png",
    "Assets/Texture/UI_rotation_moji.png",
    "Assets/Texture/UI_rotation_flash.png",
    "Assets/Texture/UI_rotation_flash_front.png",
    "Assets/Texture/UI_dush_color.png",
    "Assets/Texture/UI_dush.png",
    "Assets/Texture/UI_dush_moji_color.png",
    "Assets/Texture/UI_dush_moji.png",
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