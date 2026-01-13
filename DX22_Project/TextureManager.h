#pragma once
#include <memory>
#include <array>
#include <cstdint>
#include "Texture.h"

enum class TextureTag : uint8_t
{
    Title_BG = 0,
    LoadGauge = 1,
    Star = 2,
    GroundTile = 3,
    Stage = 4,
    SkyDome = 5,
    GaugeEmpty = 6,
    GaugeMax = 7,
	GaugeFlash = 8,
    GaugeEffect = 9,
    ScoreAtoKg = 10,
    ScoreNumber = 11,
    TimerColon = 12,
    TimerNumber = 13,
    Spin = 14,
    SpinMono = 15,
    SpinMoji = 16,
    SpinMojiMono = 17,
    SpinFlash = 18,
    SpinFront = 19,
    Dash = 20,
    DashMono = 21,
    DashMoji = 22,
    DashMojiMono = 23,
	UIFrostNoise = 24,
	EffectLv1 = 25,
	GlassShard = 26,
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
    Title_Loge,     //タイトルロゴ
    Title_Loge2,
    FadeOut,

    Count 
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