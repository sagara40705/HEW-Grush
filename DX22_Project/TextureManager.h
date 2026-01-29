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
    _SpareSlot_05 = 18,
    _SpareSlot_06 = 19,
    _SpareSlot_07 = 20,
    _SpareSlot_08 = 21,

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
    Game_UI_Action_DashIcon = 41,   // ダッシュのアイコン(四種)
    Game_UI_Action_SpinIcon = 42,   // 回転攻撃のアイコン(四種)
    Game_UI_Action_TextIcon = 43,   // アクションテキストアイコン(四種)

    Game_UI_Gaug_Lv1 = 44,	 // (完) Lv1のガラスゲージSS
    Game_UI_Gaug_Lv2 = 45,	 // (完) Lv2のガラスゲージSS
    Game_UI_Gaug_Lv3 = 46,	 // (完) Lv3のガラスゲージSS
	Game_GlassShard = 47,			 // ガラスの破片
	_SpareSlot_12 = 48,
    _SpareSlot_13 = 49,
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

    option_Back,//オプションの背景26
    option_Font1,//仕事に戻る28
    option_Font1_OK,//仕事に戻る選択29
    option_Font2,//やり直す30
    option_Font2_OK,//やり直す選択31
    option_Font3,//ステージセレクト32
    option_Font3_OK,//ステージセレクト選択33
    option_Font4,//操作方法34
    option_Font4_OK,//操作方法選択35
    option_Font5,//タイトルに戻る36
    option_Font5_OK,//タイトルに戻る選択37
    option_Font1_Back,//38
    option_Font2_Back,//39
    option_Font3_Back,//40
    option_Font1_Break,//41
    option_Font2_Break,//42
    option_Font3_Break,//43
    option_Font4_screen,//44
    option_translucent,//45

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