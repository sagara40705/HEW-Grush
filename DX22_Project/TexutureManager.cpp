#include "TextureManager.h"
#include <cassert>

std::array<std::unique_ptr<Texture>, static_cast<size_t>(TextureTag::Count)> TextureManager::s_textures;

const char* TextureManager::kTexturePath[static_cast<size_t>(TextureTag::Count)] =
{
    "Assets/Texture/Title_BackGround.png",                       //0
    "Assets/Texture/UIGauge.png",                                //1
    "Assets/Texture/Star.png",                                   //2
    "Assets/Texture/GroundTile.png",                             //3
    "Assets/Texture/stage.png",                                  //4
    "Assets/Texture/SkyDome.png",                                //5
    "Assets/Texture/lv1_0_b.png",                                //6
    "Assets/Texture/lv1_max_b.png",                              //7
    "Assets/Texture/lv1_0_g.png",                                //8
    "Assets/Texture/kirakira.png",                               //9
    "Assets/Texture/ato_kg.png",                                 //10
    "Assets/Texture/count_font_ss.png",                          //11
    "Assets/Texture/time_colon.png",                             //12
    "Assets/Texture/time_count_ss.png",                          //13
    "Assets/Texture/UI_rotation_color.png",                      //14
    "Assets/Texture/UI_rotation.png",                            //15
    "Assets/Texture/UI_rotation_moji_color.png",                 //16
    "Assets/Texture/UI_rotation_moji.png",                       //17
    "Assets/Texture/UI_rotation_flash.png",                      //18
    "Assets/Texture/UI_rotation_flash_front.png",                //19
    "Assets/Texture/UI_dush_color.png",                          //20
    "Assets/Texture/UI_dush.png",                                //21
    "Assets/Texture/UI_dush_moji_color.png",                     //22
    "Assets/Texture/UI_dush_moji.png",                           //23
    "Assets/Texture/UI_dush_flash.png",                          //24
    "Assets/Texture/UI_dush_flash_front.png",                    //25
    "Assets/Texture/UIFrostNoise08B.png",                        //26
    "Assets/Texture/effect_lv1_at_long.png",                     //27
	"Assets/Texture/Glass_Shard.png",                            //28
    "Assets/Texture/option/UI_sigotonimodoru.png",               // 仕事に戻る UI
    "Assets/Texture/option/UI_sigotonimodoru_waku.png",          // UI 枠
    "Assets/Texture/option/UI_sigotonimodoru_sentaku.png",       // 選択状態 UI
    "Assets/Texture/option/UI_sigotonimodoru_sentakuBreakID.png",//壊すためのID画像
    "Assets/Texture/proto.png",                                  //（仮）スカイドームの画像
    "Assets/Texture/bg_2048_1024.png",                           //（仮）スカイドームの画像1
    "Assets/Texture/bg_4096_2048.png",                           //（仮）スカイドームの画像2
    "Assets/Texture/Titele/title_bg.png",                        //タイトルのメイン画
    "Assets/Texture/Titele/title_hakaisuru2.png",                //タイトルの文字
    "Assets/Texture/Titele/fadein.png",                          //タイトルを割る
    "Assets/Texture/Titele/fadeinID.png",                        //タイトルを割るID
    "Assets/Texture/Titele/title_Logo.png",                      //タイトルロゴ
    "Assets/Texture/Titele/title_hakaisuru2.png"                 //タイトルロゴの斜めVar
    "Assets/Texture/fead_in_2_ss.png",                           //フェードアウト

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