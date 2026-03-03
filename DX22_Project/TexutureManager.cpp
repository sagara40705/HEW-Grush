#include "TextureManager.h"
#include <cassert>

std::array<std::unique_ptr<Texture>, static_cast<size_t>(TextureTag::Count)> TextureManager::s_textures;

const char* TextureManager::kTexturePath[static_cast<size_t>(TextureTag::Count)] =
{
    // Loading 0～7 (8)
	"Assets/Texture/Loading/Fade_In.png",           // Fade_In = 0
	"Assets/Texture/Loading/Fade_Out.png",          // Fade_Out = 1	
	"Assets/Texture/Loading/Load_BG.png",           // Load_BG = 2
    "Assets/Texture/Loading/Load_Character.png",    //Load_Character = 3		
    "Assets/Texture/Loading/Load_Gauge_Bar.png",    //Load_Gauge_Bar = 4,		    
    "Assets/Texture/Loading/Load_Gauge_Frame.png",  //Load_Gauge_Frame = 5,       
    "Assets/Texture/Loading/Load_Text.png",         //Load_Text = 6,		        
    "",//SpareSlot_01 = 7,

    /// Title 8～15(8)
    "Assets/Texture/Title/Title_BG.png",        //Title_BG = 8,
    "Assets/Texture/Title/Title_Text.png",      //Title_Text = 9,		     
    "Assets/Texture/Title/Title_Logo.png",      //Title_Logo = 10,
    "Assets/Texture/Title/Title_FadeAnim.png",  //Title_Fadein = 11,		      
    "Assets/Texture/Title/Title_FadeinID.png",  //Title_FadeinID = 12,		
    "",//SpareSlot_02 = 13,
    "",//SpareSlot_03 = 14,
    "",//SpareSlot_04 = 15,

    //// StageSelect 16～21 (6)
    "Assets/Texture/StageSelect/imege.png",            //StageSelect_BG = 16,
    "Assets/Texture/StageSelect/StageSelect_PlayerIcon.png",    //StageSelect_PlayerIcon = 17, 
    "Assets/Texture/StageSelect/StageSelect_RankS.png",//SpareSlot_05 = 18,
    "Assets/Texture/StageSelect/StageSelect_RankA.png",//SpareSlot_06 = 19,
    "Assets/Texture/StageSelect/StageSelect_RankB.png",//SpareSlot_07 = 20,
    "Assets/Texture/StageSelect/StageSelect_RankC.png",//SpareSlot_08 = 21,

    //// Tutorial 22～27 (6)
    "Assets/Texture/Tutorial/Tutorial_UI_Task.png",         //Tutorial_UI_Task = 22,
    "Assets/Texture/Tutorial/Tutorial_Road.png",            //Tutorial_Road = 23,		     
    "Assets/Texture/Tutorial/Tutorial_Targetlocation.png",  //Tutorial_Targetlocation = 24,  
    "",//SpareSlot_09 = 25,
    "",//SpareSlot_10 = 26,
    "",//SpareSlot_11 = 27,

    //// InGame 28～55 (20)
    "Assets/Texture/Game/Sky.jpg",                  //Game_Sky = 28,
    "Assets/Texture/Game/Ground.png",               //Game_Ground = 29,
    "Assets/Texture/Game/3DEffect/GradeUp.png",     //Game_3DEffect_GradeUp = 30,
    "Assets/Texture/Game/3DEffect/GradeDown.png",   //Game_3DEffect_GradeDown = 31,
    "Assets/Texture/Game/3DEffect/Hit.png",         //Game_3DEffect_Hit = 32,
    "Assets/Texture/Game/UI/Gauge/Empty.png",       //Game_UI_Gauge_Empty = 33,
    "Assets/Texture/Game/UI/Gauge/Max.png",         //Game_UI_Gauge_Max = 34,
    "Assets/Texture/Game/UI/Gauge/Fever.png",       //Game_UI_Gauge_Fever = 35,

    "Assets/Texture/Game/UI/Gauge/UpEffect.png",    //Game_UI_Gauge_UpEffect = 36, 
    "Assets/Texture/Game/UI/Timer/Number.png",      //Game_UI_Timer_Number = 37,
    "Assets/Texture/Game/UI/Timer/Colon.png",       //Game_UI_Timer_Colon = 38,
    "Assets/Texture/Game/UI/Score/Number.png",      //Game_UI_Score_Number = 39,
    "Assets/Texture/Game/UI/Score/Text.png",        //Game_UI_Score_Text = 40,
    "Assets/Texture/Game/UI/Action/ActionIcon.png", //Game_UI_Action_DushIcon = 41,
    "", //Game_UI_Action_SpinIcon = 42,
    "Assets/Texture/Game/UI/Action/TextIcon.png",   //Game_UI_Action_TextIcon = 43,

    "Assets/Texture/Game/UI/Gauge/Glass_lv1.png",//Game_UI_Gaug_Lv1 = 44,	 
    "Assets/Texture/Game/UI/Gauge/Glass_lv2.png",//Game_UI_Gaug_Lv2 = 45,	
    "Assets/Texture/Game/UI/Gauge/Glass_lv3.png",//Game_UI_Gaug_Lv3 = 46,	 
    "Assets/Texture/Game/GlassShard.png,",          //SpareSlot_12 = 47,
    "Assets/Texture/Game/UI/Score/Finish.png",// _SpareSlot_12 = 48,
    "Assets/Texture/Game/UI/Score/start.png", //_SpareSlot_13 = 49,
    "Assets/Texture/Game/Game_Shadow.png", //Game_Shadow = 50,
    "",//_SpareSlot_15 = 51,
    "",//_SpareSlot_16 = 52,
    "",//_SpareSlot_17 = 53,
    "",//_SpareSlot_18 = 54,
    "",//_SpareSlot_19 = 55,

    ////Result 56～71 (16)
    "Assets/Texture/Result/Bace.png",           //Result_Bace = 56,	
    "Assets/Texture/Result/Time/Text.png",      //Result_Time_Text = 57,
    "Assets/Texture/Result/Time/Number.png",     //Result_Time_Number = 58,
    "Assets/Texture/Result/Time/Colon.png",     //Result_Time_Colon = 59,

    "Assets/Texture/Result/Gauge_Bar.png",      //Result_Gauge_Bar = 60,	 // ゲージのバー
    "Assets/Texture/Result/Gauge_Frame.png",    //Result_Gauge_Frame = 61,	 // ゲージのフレーム
    "Assets/Texture/Result/Next_Text.png",      //Result_Next_Text = 62,	 // 次へテキスト

    "Assets/Texture/Result/Rank/S.png",         //Result_Rank_S = 63,
    "Assets/Texture/Result/Rank/A.png",         //Result_Rank_A = 64,
    "Assets/Texture/Result/Rank/B.png",         //Result_Rank_B = 65,
    "Assets/Texture/Result/Rank/C.png",         //Result_Rank_C = 66,

    "Assets/Texture/Result/Job/Pikapika.png",   //Result_Job_Pikapika = 67,
    "Assets/Texture/Result/Job/Sugoude.png",    //Result_Job_Sugoude = 68,
    "Assets/Texture/Result/Job/Tastuzin.png",   //Result_Job_tastuzin = 69,

    "", //SpareSlot_13 = 70,
    "", //SpareSlot_14 = 71,

    ////Option 72～
    "Assets/Texture/Option/Option_BG.png", //Option_BG = 72,
    "Assets/Texture/option/menu_bace.png",                      //73
    "Assets/Texture/option/GS/GS_back_to_work.png",             //74
    "Assets/Texture/option/GS/GS_back_to_work_color.png",       //75
    "Assets/Texture/option/GS/GS_retry.png",                    //76
    "Assets/Texture/option/GS/GS_retry_color.png",              //77
    "Assets/Texture/option/GS/GS_back_to_stage.png",            //78
    "Assets/Texture/option/GS/GS_back_to_stage_color.png",      //79
    "Assets/Texture/option/GS/GS_how_to.png",                   //80
    "Assets/Texture/option/GS/GS_go_title_color.png",           //81
    "Assets/Texture/option/GS/GS_go_title.png",                 //82
    "Assets/Texture/option/GS/GS_how_to_color.png",             //83
    "Assets/Texture/option/GS/GS_back_to_work_Frame.png",       //84
    "Assets/Texture/option/GS/GS_retry_Frame.png",              //85
    "Assets/Texture/option/GS/GS_back_to_stage_Frame.png",      //86
    "Assets/Texture/option/GS/GS_back_to_work_Frame_br.png",    //87
    "Assets/Texture/option/GS/GS_retry_Frame_br.png",           //88
    "Assets/Texture/option/GS/GS_back_to_stage_Frame_br.png",   //89
    "Assets/Texture/option/GS/instructions.png",                //90
    "Assets/Texture/option/GS/GS_retry_Frame_Back.png",          //91


	//ステージ選択難易度
    "Assets/Texture/StageSelect/stage_lv1.png",
    "Assets/Texture/StageSelect/stage_lv2.png",
    "Assets/Texture/StageSelect/stage_lv3.png",
    "Assets/Texture/StageSelect/StageLevel1.png",
    "Assets/Texture/StageSelect/StageLevel2.png",
    "Assets/Texture/StageSelect/StageLevel3_a.png",
    "Assets/Texture/StageSelect/StageLevel3_b.png",
    "Assets/Texture/StageSelect/StageLevel3_c.png",
    "Assets/Texture/StageSelect/Image.jpg",
    "Assets/Texture/StageSelect/bou.png",
    "Assets/Texture/StageSelect/Number.png",
    "Assets/Texture/StageSelect/Number2.png",
    "Assets/Texture/StageSelect/Number3.png",
    "Assets/Texture/StageSelect/bou2.png",
  //  "Assets/Texture/StageSelect/StageSelect_RankS.png",
   // "Assets/Texture/StageSelect/bou2.png",

	

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