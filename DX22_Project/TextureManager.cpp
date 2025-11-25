#include "TextureManager.h"
#include <cassert>

std::array<std::unique_ptr<Texture>, static_cast<int>(TextureTag::Count)> TextureManager::s_textures;

const char* TextureManager::kTexturePath[static_cast<int>(TextureTag::Count)] =
{
    "Assets/Texture/Number.png",
    "Assets/Texture/GaugeBase_Level1.png",
    "Assets/Texture/GaugeMAX_Level1.png",
    "Assets/Texture/kirakira.png",
    "Assets/Texture/RotateUI.png",
    "Assets/Texture/DashUI.png"
};

bool TextureManager::Register(TextureTag tag, const char* filePath)
{
    int index = static_cast<int>(tag);
    assert(index >= 0 && index < static_cast<int>(TextureTag::Count));

    auto tex = std::make_unique<Texture>();
    if (FAILED(tex->Create(filePath)))
        return false;

    s_textures[index] = std::move(tex);
    return true;
}

// 配列のデフォルトパスを使用して登録
bool TextureManager::RegisterAuto(TextureTag tag)
{
    return Register(tag, kTexturePath[static_cast<int>(tag)]);
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
