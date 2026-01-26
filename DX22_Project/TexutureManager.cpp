#include "TextureManager.h"
#include <cassert>

std::array<std::unique_ptr<Texture>, static_cast<size_t>(TextureTag::Count)> TextureManager::s_textures;

const char* TextureManager::kTexturePath[static_cast<size_t>(TextureTag::Count)] =
{
    // Loading 0Å`7 (8)
	"Assets/Texture/Loading/Fade_In.png",           // Fade_In = 0
	"Assets/Texture/Loading/Fade_Out.png",          // Fade_Out = 1	
	"Assets/Texture/Loading/Load_BG.png",           // Load_BG = 2
    "Assets/Texture/Loading/Load_Character.png",    //Load_Character = 3		
    "Assets/Texture/Loading/Load_Gauge_Bar.png",    //Load_Gauge_Bar = 4,		    
    "Assets/Texture/Loading/Load_Gauge_Frame.png",  //Load_Gauge_Frame = 5,       
    "Assets/Texture/Loading/Load_Gauge_Text.png",   //Load_Text = 6,		        
    "",//SpareSlot_01 = 7,

    /// Title 8Å`15(8)
    "Assets/Texture/Title/Title_BG.png",        //Title_BG = 8,
    "Assets/Texture/Title/Title_Text.png",      //Title_Text = 9,		     
    "Assets/Texture/Title/Title_Logo.png",      //Title_Logo = 10,
    "Assets/Texture/Title/Title_Fadein.png",    //Title_Fadein = 11,		      
    "Assets/Texture/Title/Title_FadeinID.png",  //Title_FadeinID = 12,		
    "",//SpareSlot_02 = 13,
    "",//SpareSlot_03 = 14,
    "",//SpareSlot_04 = 15,

    //// StageSelect 16Å`21 (6)
    "Assets/Texture/StageSelect/StageSelect_BG.png",            //StageSelect_BG = 16,
    "Assets/Texture/StageSelect/StageSelect_PlayerIcon.png",    //StageSelect_PlayerIcon = 17, 
    "",//SpareSlot_05 = 18,
    "",//SpareSlot_06 = 19,
    "",//SpareSlot_07 = 20,
    "",//SpareSlot_08 = 21,

    //// Tutorial 22Å`27 (6)
    "Assets/Texture/Tutorial/Tutorial_UI_Task.png",         //Tutorial_UI_Task = 22,
    "Assets/Texture/Tutorial/Tutorial_Road.png",            //Tutorial_Road = 23,		     
    "Assets/Texture/Tutorial/Tutorial_Targetlocation.png",  //Tutorial_Targetlocation = 24,  
    "",//SpareSlot_09 = 25,
    "",//SpareSlot_10 = 26,
    "",//SpareSlot_11 = 27,

    //// InGame 28Å`55 (20)
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
    "Assets/Texture/Game/UI/Action/DushIcon.png",   //Game_UI_Action_DushIcon = 41,
    "Assets/Texture/Game/UI/Action/SpinIcon.png",   //Game_UI_Action_SpinIcon = 42,
    "Assets/Texture/Game/UI/Action/textIcon.png",   //Game_UI_Action_TextIcon = 43,

    "",//Game_UI_Gaug_Lv1 = 44,	 // (äÆ) Lv1ÇÃÉKÉâÉXÉQÅ[ÉWSS
    "",//Game_UI_Gaug_Lv2 = 45,	 // (äÆ) Lv2ÇÃÉKÉâÉXÉQÅ[ÉWSS
    "",//Game_UI_Gaug_Lv3 = 46,	 // (äÆ) Lv3ÇÃÉKÉâÉXÉQÅ[ÉWSS
    "Assets/Texture/Game/GlassShard.png,",          //SpareSlot_12 = 47,
    "",// _SpareSlot_12 = 48,
    "",//_SpareSlot_13 = 49,
    "",//_SpareSlot_14 = 50,
    "",//_SpareSlot_15 = 51,
    "",//_SpareSlot_16 = 52,
    "",//_SpareSlot_17 = 53,
    "",//_SpareSlot_18 = 54,
    "",//_SpareSlot_19 = 55,

    ////Result 56Å`71 (16)
    "Assets/Texture/Result/Bace.png",           //Result_Bace = 56,	
    "Assets/Texture/Result/Time/Text.png",      //Result_Time_Text = 57,
    "Assets/Texture/Result/Time/Nuber.png",     //Result_Time_Number = 58,
    "Assets/Texture/Result/Time/Colon.png",     //Result_Time_Colon = 59,

    "Assets/Texture/Result/Gauge_Bar.png",      //Result_Gauge_Bar = 60,	 // ÉQÅ[ÉWÇÃÉoÅ[
    "Assets/Texture/Result/Gauge_Frame.png",    //Result_Gauge_Frame = 61,	 // ÉQÅ[ÉWÇÃÉtÉåÅ[ÉÄ
    "Assets/Texture/Result/Next_Text.png",      //Result_Next_Text = 62,	 // éüÇ÷ÉeÉLÉXÉg

    "Assets/Texture/Result/Rank/S.png",         //Result_Rank_S = 63,
    "Assets/Texture/Result/Rank/A.png",         //Result_Rank_A = 64,
    "Assets/Texture/Result/Rank/B.png",         //Result_Rank_B = 65,
    "Assets/Texture/Result/Rank/C.png",         //Result_Rank_C = 66,

    "Assets/Texture/Result/Job/Pikapika.png",   //Result_Job_Pikapika = 67,
    "Assets/Texture/Result/Job/Sugoude.png",    //Result_Job_Sugoude = 68,
    "Assets/Texture/Result/Job/Tastuzin.png",   //Result_Job_tastuzin = 69,

    "", //SpareSlot_13 = 70,
    "", //SpareSlot_14 = 71,

    ////Option 72Å`
    "Assets/Texture/Option/Option_BG.png", //Option_BG = 72,
};

// îzóÒÇÃÉfÉtÉHÉãÉgÉpÉXÇégópÇµÇƒìoò^
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