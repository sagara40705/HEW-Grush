#pragma once
#include <memory>
#include <array>
#include "Texture.h"

enum class TextureTag : int
{
    Number,          // Assets/Texture/Number.png
    GaugeMaxLv1,     // Assets/Texture/GaugeMAX_Level1.png

    Count
};

class TextureManager
{
private:
    static std::array<std::unique_ptr<Texture>, static_cast<int>(TextureTag::Count)> s_textures;
    static const char* kTexturePath[static_cast<int>(TextureTag::Count)];

public:
    /// 成功時 true, 失敗時 false
    static bool Register(TextureTag tag, const char* filePath);
    static bool RegisterAuto(TextureTag tag);   // ファイルパスを自動参照

    /// 取得：tag に対応する Texture のポインタを返す（なければ nullptr）
    static Texture* Get(TextureTag tag);

    /// 全削除：登録されているすべての Texture を解放
    static void Clear();
};