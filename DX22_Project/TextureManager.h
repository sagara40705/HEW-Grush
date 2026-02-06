#pragma once
#include <memory>
#include <array>
#include <cstdint>
#include "Texture.h"

enum class TextureTag : uint8_t
{
    // Loading 0～7 (8)
    Fade_In = 0,		            // フェードインSS
    Fade_Out = 1,		            // フェードアウトSS
    Load_BG = 2,		            // 背景
    Load_Character = 3,		        // キャラクターアニメーションSS
    Load_Gauge_Bar = 4,		        // ゲージの本体
    Load_Gauge_Frame = 5,           // ゲージのフレーム
    Load_Text = 6,		            // Now loadingの文字
    _SpareSlot_01 = 7,

    // Title 8～15(8)
    Title_BG = 8,		            // 背景
    Title_Text = 9,		            // 破壊する!の文字
    Title_Logo = 10,		        // タイトルロゴ
    Title_Fadein = 11,		        // 演出用
    Title_FadeinID = 12,		    // 演出用
    _SpareSlot_02 = 13,
    _SpareSlot_03 = 14,
    _SpareSlot_04 = 15,

    // StageSelect 16～21 (6)
    StageSelect_BG = 16,		    // 背景
    StageSelect_PlayerIcon = 17,    // プレイヤーアイコン
    StageSelect_Rank_S = 18,
    StageSelect_Rank_A = 19,
    StageSelect_Rank_B = 20,
    StageSelect_Rank_C = 21,

    // Tutorial 22～27 (6)
    Tutorial_UI_Task = 22,	        // チュートリアルタスク
    Tutorial_Road = 23,		        // 道
    Tutorial_Targetlocation = 24,   // 目標地点
    _SpareSlot_09 = 25,
    _SpareSlot_10 = 26,
    _SpareSlot_11 = 27,

    // InGame 28～55 (28)
    Game_Sky = 28,		            // スカイドーム用
    Game_Ground = 29,		        // 地面
    Game_3DEffect_GradeUp = 30,	    // ゲージUP時のエフェクトSS
    Game_3DEffect_GradeDown = 31,   // ゲージDown時のエフェクトSS
    Game_3DEffect_Hit = 32,	        // ヒットエフェクト
    Game_UI_Gauge_Empty = 33,	    // (仮)空のゲージ
    Game_UI_Gauge_Max = 34,         // (仮)ゲージMax
    Game_UI_Gauge_Fever = 35,	    // フィーバー状態のゲージ

    Game_UI_Gauge_UpEffect = 36,    // ゲージUP時の2DエフェクトSS
    Game_UI_Timer_Number = 37,	    // タイマーの数字
    Game_UI_Timer_Colon = 38,	    // タイマーのコロン
    Game_UI_Score_Number = 39,      // スコアの数字
    Game_UI_Score_Text = 40,        // スコアのテキスト(あと...kg)
    Game_UI_Action_Icon = 41,       // ダッシュのアイコン(四種)&回転攻撃のアイコン(四種)
    Game_UI_Action_SpinIcon = 42,   // 回転攻撃のアイコン(四種)
    Game_UI_Action_TextIcon = 43,   // アクションテキストアイコン(四種)

    Game_UI_Gaug_Lv1 = 44,	 // (完) Lv1のガラスゲージSS
    Game_UI_Gaug_Lv2 = 45,	 // (完) Lv2のガラスゲージSS
    Game_UI_Gaug_Lv3 = 46,	 // (完) Lv3のガラスゲージSS
	Game_GlassShard = 47,			 // ガラスの破片
	Game_UI_Finish = 48,
    Game_UI_Start = 49,
    _SpareSlot_14 = 50,
    _SpareSlot_15 = 51,
    _SpareSlot_16 = 52,
    _SpareSlot_17 = 53,
    _SpareSlot_18 = 54,
    _SpareSlot_19 = 55,

    //Result 56～71 (16)
    Result_Bace = 56,		 // ベースの背景
    Result_Time_Text = 57,	 // タイムの文字
    Result_Time_Number = 58,	 // タイムの数字
    Result_Time_Colon = 59,	 // タイムのコロン

    Result_Gauge_Bar = 60,	 // ゲージのバー
    Result_Gauge_Frame = 61,	 // ゲージのフレーム
    Result_Next_Text = 62,	 // 次へテキスト

    Result_Rank_S = 63,		 // ランク
    Result_Rank_A = 64,		 // ランク
    Result_Rank_B = 65,		 // ランク
    Result_Rank_C = 66,		 // ランク

    Result_Job_Pikapika = 67,	 // 役職の文字(ピカピカ・すごうで・たつじんの三種)
    Result_Job_Sugoude = 68,	 // 役職の文字(ピカピカ・すごうで・たつじんの三種)
    Result_Job_Tastuzin = 69,	 // 役職の文字(ピカピカ・すごうで・たつじんの三種)

    _SpareSlot_20 = 70,
    _SpareSlot_21 = 71,

    //Option 72～
    Option_BG = 72,
    option_Back = 73,
    option_Font1 = 74,
    option_Font1_OK = 75,
    option_Font2 = 76,
    option_Font2_OK = 77,
    option_Font3 = 78,
    option_Font3_OK = 79,
    option_Font4 = 80,
    option_Font4_OK = 81,
    option_Font5 = 82,
    option_Font5_OK = 83,
    option_Font1_Back = 84,
    option_Font2_Back = 85,
    option_Font3_Back = 86,
    option_Font1_Break = 87,
    option_Font2_Break = 88,
    option_Font3_Break = 89,
    option_Font4_screen = 90,
    option_translucent = 91,

	stageLv1 = 92,
	stageLv2 = 93,
	stageLv3 = 94,
	stagelevel_Easy = 95,
	stagelevel_Normal = 96,
	Stagelevel_Hard_a = 97,
	Stagelevel_Hard_b = 98,
    Stagelevel_Hard_c = 99,
    StageImage = 100,
	bou = 101,
	Number = 102,
	Number2 = 103,
	Number3 = 104,
	bou2 = 105,

   
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