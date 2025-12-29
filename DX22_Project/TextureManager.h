#pragma once
#include <memory>      // unique_ptr を使用するため
#include <array>       // 固定長コンテナ array を使用するため
#include <cstdint>     // uint8_t の定義用
#include "Texture.h"   // Texture クラスを参照するため

// TextureTag:
// 読み込むテクスチャ種類を識別するための列挙型。
// 各値は TextureManager 内の配列インデックスと 1 対 1 対応します。
// uint8_t で宣言することでメモリ消費を最小化。
enum class TextureTag : uint8_t
{
    Title_BG,       // タイトル背景
    FadeOut,
    LoadGauge,      // UI ロードゲージ
    Star,           // 星アイコン
    GaugeEmpty,     // ゲージ空状態画像
    GaugeMax,       // ゲージ最大状態画像
    GaugeEffect,    // ゲージに使うエフェクト素材
    ScoreAtoKg,     // スコア UI: 「ato kg」表示
    ScoreNumber,    // スコア用数字フォント
    TimerColon,     // タイマー用コロン画像
    TimerNumber,    // タイマー数字フォント（スプライトシート）
    UI_font,        // メニュー UI テキスト画像
    UI_Frame,       // メニュー UI 枠画像
    UI_Break,       // UI 選択時などのハイライト画像
    UI_BreakID,     //UIのシェーダーアニメーションを作るためのID用画像
    Background_Game,//ゲームの背景用
    Background_Game2,//ゲームの背景用
    Background_Game3,//ゲームの背景用
    Title_Main,      //タイトルのメイン画像
    Title_Font,     //タイトルの文字
    Title_Break,    //タイトルを割る
    Title_BreakID,  //タイトル割るID
    Count           // 列挙数（配列確保のためだけに使用）
};

// TextureManager:
// 静的クラスとして動作するテクスチャ管理クラス。
// 各 TextureTag に対応する Texture インスタンスを内部で保持します。
// 読み込み、取得、解放の操作をタグ指定で統一できます。
class TextureManager
{
private:
    // s_textures:
    // Texture インスタンスを unique_ptr で保持する静的配列。
    // 配列の要素数は TextureTag::Count で決定されます。
    static std::array<std::unique_ptr<Texture>, static_cast<size_t>(TextureTag::Count)> s_textures;

    // kTexturePath:
    // TextureTag ごとに対応する画像ファイルパスを格納した静的配列。
    // Load() 内で自動的に参照されます。
    static const char* kTexturePath[static_cast<size_t>(TextureTag::Count)];

public:
    // Load:
    // 指定した tag のテクスチャを読み込みます。
    // すでに読み込まれている場合は false を返します。
    // 戻り値: 読み込み成功で true, 失敗で false。
    static bool Load(TextureTag tag);

    // Unload:
    // 指定した tag のテクスチャを破棄します。
    // unique_ptr を reset() することでメモリを解放します。
    static void Unload(TextureTag tag);

    // Get:
    // 指定 tag に対応する Texture* を返します。
    // 未読み込みの場合は nullptr が返ります。
    static Texture* Get(TextureTag tag);

    // Clear:
    // 全てのテクスチャを解放します。
    // シーン切り替えやアプリ終了時に使用される想定。
    static void Clear();
};