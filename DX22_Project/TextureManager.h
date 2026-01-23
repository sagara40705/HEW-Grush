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
    SpareSlot_01 = 7,

    // Title 8～15(8)
    Title_BG = 8,		            // 背景
    Title_Text = 9,		            // 破壊する!の文字
    Title_Logo = 10,		        // タイトルロゴ
    Title_Fadein = 11,		        // 演出用
    Title_FadeinID = 12,		    // 演出用
    SpareSlot_02 = 13,
    SpareSlot_03 = 14,
    SpareSlot_04 = 15,

    // StageSelect 16～21 (6)
    StageSelect_BG = 16,		    // 背景
    StageSelect_PlayerIcon = 17,    // プレイヤーアイコン
    SpareSlot_05 = 18,
    SpareSlot_06 = 19,
    SpareSlot_07 = 20,
    SpareSlot_08 = 21,

    // Tutorial 22～27 (6)
    Tutorial_UI_Task = 22,	        // チュートリアルタスク
    Tutorial_Road = 23,		        // 道
    Tutorial_Targetlocation = 24,   // 目標地点
    SpareSlot_09 = 25,
    SpareSlot_10 = 26,
    SpareSlot_11 = 27,

    // InGame 28～47 (20)
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
    Game_UI_Action_DushIcon = 41,   // ダッシュのアイコン(四種)
    Game_UI_Action_SpinIcon = 42,   // 回転攻撃のアイコン(四種)
    Game_UI_Action_TextIcon = 43,   // アクションテキストアイコン(四種)

    Game_UI_Gaug_Lv1 = 44,	 // (完) Lv1のガラスゲージSS
    Game_UI_Gaug_Lv2 = 45,	 // (完) Lv2のガラスゲージSS
    Game_UI_Gaug_Lv3 = 46,	 // (完) Lv3のガラスゲージSS
    SpareSlot_12 = 47,

    //Result 48～63 (16)
    Result_Bace = 48,		 // ベースの背景
    Result_Time_Text = 49,	 // タイムの文字
    Result_Time_Number = 50,	 // タイムの数字
    Result_Time_Colon = 51,	 // タイムのコロン

    Result_Gauge_Bar = 52,	 // ゲージのバー
    Result_Gauge_Frame = 53,	 // ゲージのフレーム
    Result_Next_Text = 54,	 // 次へテキスト

    Result_Rank_S = 55,		 // ランク
    Result_Rank_A = 56,		 // ランク
    Result_Rank_B = 57,		 // ランク
    Result_Rank_C = 58,		 // ランク

    Result_Job_Pikapika = 59,	 // 役職の文字(ピカピカ・すごうで・たつじんの三種)
    Result_Job_Sugoude = 60,	 // 役職の文字(ピカピカ・すごうで・たつじんの三種)
    Result_Job_tastuzin = 61,	 // 役職の文字(ピカピカ・すごうで・たつじんの三種)

    SpareSlot_13 = 62,
    SpareSlot_14 = 63,

    //Option 64～
    Option_BG = 64,

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