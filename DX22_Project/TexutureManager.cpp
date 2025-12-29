#include "TextureManager.h"
#include <cassert>

// TextureManager は Texture の管理クラスです。
// 固定長配列にテクスチャを保持し、タグをキーとして管理します。

// s_textures: Texture のインスタンスを格納する配列。
// unique_ptr で自動管理され、未使用時は nullptr になります。
std::array<std::unique_ptr<Texture>, static_cast<size_t>(TextureTag::Count)> TextureManager::s_textures;

// kTexturePath: 各 TextureTag に対応する画像ファイルのパス一覧。
// インデックス順が TextureTag と一致している必要があります。
const char* TextureManager::kTexturePath[static_cast<size_t>(TextureTag::Count)] =
{
    "Assets/Texture/Title_BackGround.png",            // Title の背景画像
    "Assets/Texture/fead_in_2_ss.png",                //フェードアウト
    "Assets/Texture/UIGauge.png",                     // UI のゲージ画像
    "Assets/Texture/Star.png",                        // 星アイコン
    "Assets/Texture/lv1_0_b.png",                     // レベル1 (0状態)
    "Assets/Texture/lv1_max_b.png",                   // レベル1 (MAX状態)
    "Assets/Texture/kirakira.png",                    // きらきらエフェクト
    "Assets/Texture/ato_kg.png",                      // 残り時間 UI
    "Assets/Texture/count_font_ss.png",               // 数字フォント（スプライトシート）
    "Assets/Texture/time_colon.png",                  // 時間コロン
    "Assets/Texture/time_count_ss.png",               // 時間カウント（スプライトシート）
    "Assets/Texture/option/UI_sigotonimodoru.png",    // 仕事に戻る UI
    "Assets/Texture/option/UI_sigotonimodoru_waku.png",// UI 枠
    "Assets/Texture/option/UI_sigotonimodoru_sentaku.png",// 選択状態 UI
    "Assets/Texture/option/UI_sigotonimodoru_sentakuBreakID.png",//壊すためのID画像
    "Assets/Texture/proto.png",                           //（仮）スカイドームの画像
    "Assets/Texture/bg_2048_1024.png",//（仮）スカイドームの画像1
    "Assets/Texture/bg_4096_2048.png",//（仮）スカイドームの画像2
    "Assets/Texture/Titele/title_bg.png",//タイトルのメイン画
    "Assets/Texture/Titele/title_hakaisuru.png",//タイトルの文字
    "Assets/Texture/Titele/fadein.png",//タイトルを割る
    "Assets/Texture/Titele/fadeinID.png"//タイトルを割るID
};

// Load: 指定された TextureTag のテクスチャを読み込む。
// すでに読み込まれている場合は false を返し処理を行わない。
bool TextureManager::Load(TextureTag tag)
{
    // 既に読み込み済みの場合は何もしない。
    if (s_textures[static_cast<size_t>(tag)]) return false;

    int index = static_cast<int>(tag);
    const char* filePath = kTexturePath[static_cast<size_t>(tag)];

    // index が配列の範囲外にならないようにチェックする。
    assert(index >= 0 && index < static_cast<int>(TextureTag::Count));

    // Texture インスタンスを生成し画像を読み込む。
    auto tex = std::make_unique<Texture>();//テクスチャのNEWを作る
    if (FAILED(tex->Create(filePath)))
        return false; // 読み込み失敗時は false を返す。

    // 読み込み成功したので配列に登録する。
    s_textures[index] = std::move(tex);
    return true;
}

// Unload: 指定タグのテクスチャを解放する。
// unique_ptr の reset によりメモリが解放される。
void TextureManager::Unload(TextureTag tag)
{
    s_textures[static_cast<size_t>(tag)].reset();
}

// Get: 指定タグの Texture を取得する。
// 読み込まれていない場合は nullptr が返る。
Texture* TextureManager::Get(TextureTag tag)
{
    return s_textures[static_cast<int>(tag)].get();
}

// Clear: 全テクスチャを解放する。
// シーン切り替えやアプリ終了時に使用される想定。
void TextureManager::Clear()
{
    for (auto& t : s_textures)
    {
        t.reset(); // 各 Texture を解放
    }
}