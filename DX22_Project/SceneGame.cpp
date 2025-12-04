#include "SceneGame.h"
#include <DirectXMath.h>
#include "Defines.h"
#include "World.h"
#include "AllComponet.h"
#include "PhysicsSystem.h"
#include "SampleMapData.h"
#include "Model.h"
#include "LineManager.h"
#include "RenderSystem.h"
#include "Game.h"
#include "TextureManager.h"
#include "ModelManager.h"
//#define CONIOEX
#include"conioex.h"

World* W = nullptr;
INT_PTR g_GameBGM;

namespace
{
    // ブロック共通処理
    GameObject& SpawnCubeBlock(const DirectX::XMFLOAT3& pos,
        const DirectX::XMFLOAT3& scale = Vec3One,
        bool isStaticCollider = true)
    {
        GameObject& o = W->CreateGameObject();
        o.AddComponent<Transform>(pos, Vec3Zero, scale);
        o.AddComponent<MeshFilter>(MeshFilter::Type::Cube);
        o.AddComponent<MeshRender>();
        o.AddComponent<BoxCollider>();
        o.AddComponent<GlassScript>();
        o.SetTag(Tag::Glass);
        return o;
    }

    // 三次元マップ(0/1)からブロック群を生成
    template<size_t Y, size_t Z, size_t X>
    void BuildBlocksFromMap(const int(&map)[Y][Z][X],
        float unit = kUnit,
        const DirectX::XMFLOAT3& scale = Vec3One,
        bool isStaticCollider = true)
    {
        for (size_t y = 0; y < Y; ++y)
        {
            for (size_t z = 0; z < Z; ++z)
            {
                for (size_t x = 0; x < X; ++x)
                {
                    if (map[y][z][x] == 0) continue;

                    DirectX::XMFLOAT3 pos
                    {
                        static_cast<float>(x) * unit,
                        static_cast<float>(y) * unit,
                        static_cast<float>(z) * unit
                    };
                    SpawnCubeBlock(pos, scale, isStaticCollider);
                }
            }
        }
    }
}

SceneGame::SceneGame()
{

   g_GameBGM = opensound("Sound/maou_bgm_cyber41.mp3");    // オープン＆初期化
   if (g_GameBGM)
   {
       playsound(g_GameBGM, 1);
   }

    W = &Game::GetWorld();

    Debug::Init();
    RegisterTexture();
    RegisterModel();

    GameObject& camera = W->CreateGameObject();
    camera.AddComponent<Transform>(Camera::initPos(), Camera::initRotate(), Vector3::zero());
    camera.AddComponent<Camera>();
    LineManager::Init();

    GameObject& player = W->CreateGameObject();
    player.AddComponent<Transform>(Vector3(0, 2, 0), Vector3::zero(), Vector3(1, 1, 1));
    player.AddComponent<MeshFilter>(MeshFilter::Type::Cylinder);
    player.AddComponent<MeshRender>(1);
    player.AddComponent<BoxCollider>(true, Vector3(0, 0, 0), Vector3(1, 1, 1));
    // player.AddComponent<SphereCollider>(0.5f, false);
    // player.AddComponent<CapsuleCollider>(0.01f, 0.5f, false);
    player.AddComponent<Rigidbody>();
    player.AddComponent<PlayerMove>();
    player.AddComponent<PlayerAction>();
    camera.AddComponent<CameraFollow>(&player.GetComponent<Transform>());
    player.SetTag(Tag::Player);
    // LineManager::AddBoxCollider(&player);

    GameObject& hammer = W->CreateGameObject();
    hammer.AddComponent<Transform>(Vector3(0, 0, 1.5f), Vector3::zero(), kDefaultHummerScale);
    hammer.TryGetComponent<Transform>()->SetParent(&player.GetComponent<Transform>());
    hammer.AddComponent<MeshFilter>(MeshFilter::Type::Cube);
    hammer.AddComponent<MeshRender>(1);
    auto& hbc = hammer.AddComponent<BoxCollider>(false); hbc.isTrigger = true;
    hammer.AddComponent<HummerScript>(player.TryGetComponent<Transform>());
    hammer.SetTag(Tag::Weapon);

    GameObject& floor = W->CreateGameObject();
    floor.AddComponent<Transform>(Vector3(0, -1, 0), Vector3::zero(), Vector3(50, 0.1, 50));
    floor.AddComponent<MeshFilter>(MeshFilter::Type::Plane);
    floor.AddComponent<MeshRender>();
    floor.AddComponent<BoxCollider>();
    floor.SetTag(Tag::Ground);

    //BuildBlocksFromMap(map, /*unit=*/kUnit, /*scale=*/blockScale, /*isStaticCollider=*/true);

    //GameObject& karubi = W->CreateGameObject();
    //karubi.AddComponent<Transform>(DirectX::XMFLOAT3{ 2,0.5,2 }, Vec3Zero, Vec3One);
    //karubi.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Karubi));
    //karubi.AddComponent<ModelRenderer>();
    //karubi.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0,0.75f,0 }, DirectX::XMFLOAT3{ 1, 1.5, 1 });
    //LineManager::AddBoxCollider(&karubi);

    CreateProtStage();
    //CreateGlassModel();
    CreateTimerUi();
    CreateScoreUi();
    CreateActionUi();

    GameObject& gaugeFrame = W->CreateGameObject();
    CanvasUI& gaugeFrameUI = gaugeFrame.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::GaugeFrame));

    GameObject& gauge = W->CreateGameObject();
    CanvasUI& gaugeUI = gauge.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::GaugeMaxLv1));

    GameObject& GaEf = W->CreateGameObject();
    CanvasUI& gaefUI = GaEf.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::GaugeEffect));

    GameObject& gaugeUiManagerObj = W->CreateGameObject();
    gaugeUiManagerObj.AddComponent<GlassGauge>(&gaugeFrameUI , &gaugeUI, &gaefUI);

   
    W->Start();
   
}
SceneGame::~SceneGame()
{
    W->ClearAll();
    W = nullptr;
    TextureManager::Clear();
    closesound(g_GameBGM);
}
void SceneGame::Update()
{
  /*  if (g_GameBGM)
    {
        updatesound(g_GameBGM);
    }*/
    W->Update();
}
void SceneGame::Draw()
{
    W->Draw();
}

void SceneGame::RegisterTexture()
{
    TextureManager::RegisterAuto(TextureTag::Number);
    TextureManager::RegisterAuto(TextureTag::GaugeFrame);
    TextureManager::RegisterAuto(TextureTag::GaugeMaxLv1);
    TextureManager::RegisterAuto(TextureTag::GaugeEffect);
    TextureManager::RegisterAuto(TextureTag::SpinUI);
    TextureManager::RegisterAuto(TextureTag::DashUI);
}

void SceneGame::RegisterModel()
{
    ModelManager::RegisterAuto(ModelTag::Karubi);
    ModelManager::RegisterAuto(ModelTag::Swing);
    ModelManager::RegisterAuto(ModelTag::Benti);
    ModelManager::RegisterAuto(ModelTag::Ufo);
    ModelManager::RegisterAuto(ModelTag::ProtPlay);
}

void SceneGame::CreateProtStage()
{
    //最後に壊させる
    //最後に一番大きいガラスを割ってみよう！(最大強化できた時)
    //UFOを割ったらクリア
    GameObject& ufo1 = W->CreateGameObject();
    ufo1.AddComponent<Transform>(DirectX::XMFLOAT3{ 10, -0.5f,10 }, Vec3Zero, Vec3One);
    ufo1.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Ufo));
    ufo1.AddComponent<ModelRenderer>();
    ufo1.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0,2,0 }, DirectX::XMFLOAT3{ 4,4 , 4 });
    LineManager::AddBoxCollider(&ufo1);
    ufo1.AddComponent<GlassScript>(GlassGrade::Grade_3);
    ufo1.SetTag(Tag::Glass);

    //二番目に壊させる
    //ハンマーがパワーアップした！大きいガラスを割ったほうがゲージがより増えやすい！
    //たくさん壊して最大強化させよう！
    GameObject& benti1 = W->CreateGameObject();
    benti1.AddComponent<Transform>(DirectX::XMFLOAT3{ -10,-0.9f,10 }, Vec3Zero, Vector3( 1.2f, 1.2f, 1.2f));
    benti1.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Benti));
    benti1.AddComponent<ModelRenderer>();
    benti1.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0,1,0 }, DirectX::XMFLOAT3{ 4,2 , 2 });
    LineManager::AddBoxCollider(&benti1);
    benti1.AddComponent<GlassScript>(GlassGrade::Grade_2);
    benti1.SetTag(Tag::Glass);
    

    GameObject& benti2 = W->CreateGameObject();
    benti2.AddComponent<Transform>(DirectX::XMFLOAT3{ 20,-0.9f,-16 }, Vec3Zero, Vector3( 1.2f, 1.2f, 1.2f));
    benti2.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Benti));
    benti2.AddComponent<ModelRenderer>();
    benti2.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0,1,0 }, DirectX::XMFLOAT3{ 4,2 , 2 });
    LineManager::AddBoxCollider(&benti2);
    benti2.AddComponent<GlassScript>(GlassGrade::Grade_2);
    benti2.SetTag(Tag::Glass);

    GameObject& benti3 = W->CreateGameObject();
    benti3.AddComponent<Transform>(DirectX::XMFLOAT3{ 11,-0.9f,-13 }, Vec3Zero, Vector3( 1.2f, 1.2f, 1.2f));
    benti3.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Benti));
    benti3.AddComponent<ModelRenderer>();
    benti3.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0,1,0 }, DirectX::XMFLOAT3{ 4,2 , 2 });
    LineManager::AddBoxCollider(&benti3);
    benti3.AddComponent<GlassScript>(GlassGrade::Grade_2);
    benti3.SetTag(Tag::Glass);

    //一番初めに壊させる。
    // ここで簡単な操作説明
    //<内容文>物を壊すとゲージがたまる！ゲージがMAXになるとハンマーが強化され、より大きなガラスを破壊できるようになるぞ！
    //もっと小さなガラスを割ってゲージをためてみよう！
    GameObject& swing1 = W->CreateGameObject();
    swing1.AddComponent<Transform>(DirectX::XMFLOAT3{ -7,-0.9f,-8 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    swing1.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Swing));
    swing1.AddComponent<ModelRenderer>();
    swing1.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0, 0.7f, 0.2f }, DirectX::XMFLOAT3{ 1.5f, 1.5f , 1.5f });
    LineManager::AddBoxCollider(&swing1);
    swing1.AddComponent<GlassScript>(GlassGrade::Grade_1);
    swing1.SetTag(Tag::Glass);

    GameObject& swing2 = W->CreateGameObject();
    swing2.AddComponent<Transform>(DirectX::XMFLOAT3{ -12,-0.9f,-2 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    swing2.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Swing));
    swing2.AddComponent<ModelRenderer>();
    swing2.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0, 0.7f, 0.2f }, DirectX::XMFLOAT3{ 1.5f, 1.5f , 1.5f });
    LineManager::AddBoxCollider(&swing2);
    swing2.AddComponent<GlassScript>(GlassGrade::Grade_1);
    swing2.SetTag(Tag::Glass);

    GameObject& swing3 = W->CreateGameObject();
    swing3.AddComponent<Transform>(DirectX::XMFLOAT3{ -22,-0.9f,-13 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    swing3.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Swing));
    swing3.AddComponent<ModelRenderer>();
    swing3.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0, 0.7f, 0.2f }, DirectX::XMFLOAT3{ 1.5f, 1.5f , 1.5f });
    LineManager::AddBoxCollider(&swing3);
    swing3.AddComponent<GlassScript>(GlassGrade::Grade_1);
    swing3.SetTag(Tag::Glass);


    //物が少なくてさみしいので適当に置きます↓（削除OK）
    GameObject& player1 = W->CreateGameObject();
    player1.AddComponent<Transform>(DirectX::XMFLOAT3{ -12,-0.9f,8 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    player1.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
    player1.AddComponent<ModelRenderer>();
    player1.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
    LineManager::AddBoxCollider(&player1);
    player1.AddComponent<GlassScript>(GlassGrade::Grade_1);
    player1.SetTag(Tag::Glass);

    GameObject& player2 = W->CreateGameObject();
    player2.AddComponent<Transform>(DirectX::XMFLOAT3{ 5,-0.9f,2 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    player2.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
    player2.AddComponent<ModelRenderer>();
    player2.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
    LineManager::AddBoxCollider(&player2);
    player2.AddComponent<GlassScript>(GlassGrade::Grade_1);
    player2.SetTag(Tag::Glass);

    GameObject& player3 = W->CreateGameObject();
    player3.AddComponent<Transform>(DirectX::XMFLOAT3{ -20,-0.9f,-20 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    player3.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
    player3.AddComponent<ModelRenderer>();
    player3.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
    LineManager::AddBoxCollider(&player3);
    player3.AddComponent<GlassScript>(GlassGrade::Grade_1);
    player3.SetTag(Tag::Glass);

    GameObject& player4 = W->CreateGameObject();
    player4.AddComponent<Transform>(DirectX::XMFLOAT3{ -16,-0.9f,-16 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    player4.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
    player4.AddComponent<ModelRenderer>();
    player4.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
    LineManager::AddBoxCollider(&player4);
    player4.AddComponent<GlassScript>(GlassGrade::Grade_1);
    player4.SetTag(Tag::Glass);

    GameObject& player5 = W->CreateGameObject();
    player5.AddComponent<Transform>(DirectX::XMFLOAT3{ 13,-0.9f,2 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    player5.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
    player5.AddComponent<ModelRenderer>();
    player5.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
    LineManager::AddBoxCollider(&player5);
    player5.AddComponent<GlassScript>(GlassGrade::Grade_1);
    player5.SetTag(Tag::Glass);

    GameObject& player6 = W->CreateGameObject();
    player6.AddComponent<Transform>(DirectX::XMFLOAT3{ 22,-0.9f,8 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    player6.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
    player6.AddComponent<ModelRenderer>();
    player6.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
    LineManager::AddBoxCollider(&player6);
    player6.AddComponent<GlassScript>(GlassGrade::Grade_1);
    player6.SetTag(Tag::Glass);

    GameObject& player7 = W->CreateGameObject();
    player7.AddComponent<Transform>(DirectX::XMFLOAT3{ -17,-0.9f,15 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    player7.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
    player7.AddComponent<ModelRenderer>();
    player7.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
    LineManager::AddBoxCollider(&player7);
    player7.AddComponent<GlassScript>(GlassGrade::Grade_1);
    player7.SetTag(Tag::Glass);

    GameObject& player8 = W->CreateGameObject();
    player8.AddComponent<Transform>(DirectX::XMFLOAT3{ 6,-0.9f,17 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    player8.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
    player8.AddComponent<ModelRenderer>();
    player8.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
    LineManager::AddBoxCollider(&player8);
    player8.AddComponent<GlassScript>(GlassGrade::Grade_1);
    player8.SetTag(Tag::Glass);

    GameObject& player9 = W->CreateGameObject();
    player9.AddComponent<Transform>(DirectX::XMFLOAT3{ 20,-0.9f,-12 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    player9.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
    player9.AddComponent<ModelRenderer>();
    player9.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
    LineManager::AddBoxCollider(&player9);
    player9.AddComponent<GlassScript>(GlassGrade::Grade_1);
    player9.SetTag(Tag::Glass);

    GameObject& player10 = W->CreateGameObject();
    player10.AddComponent<Transform>(DirectX::XMFLOAT3{ 14,-0.9f,-19 }, Vec3Zero, Vector3(0.8f, 0.8f, 0.8f));
    player10.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::ProtPlay));
    player10.AddComponent<ModelRenderer>();
    player10.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ -0.2f, 1, 0 }, DirectX::XMFLOAT3{ 1, 2, 1 });
    LineManager::AddBoxCollider(&player10);
    player10.AddComponent<GlassScript>(GlassGrade::Grade_1);
    player10.SetTag(Tag::Glass);
}

void SceneGame::CreateGlassModel()
{
    GameObject& ufo = W->CreateGameObject();
    ufo.AddComponent<Transform>(DirectX::XMFLOAT3{ 4,0.5,4 }, Vec3Zero, Vec3One);
    ufo.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Ufo));
    ufo.AddComponent<ModelRenderer>();
    ufo.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0,0,0 }, DirectX::XMFLOAT3{ 2, 2, 2 });
    LineManager::AddBoxCollider(&ufo);
}

void SceneGame::SetPlayerAndHummer()
{
}

void SceneGame::CreateTimerUi()
{
    GameObject& tm = W->CreateGameObject();
    CanvasUI& tmUI = tm.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::Number));

    GameObject& om = W->CreateGameObject();
    CanvasUI& omUI = om.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::Number));

    GameObject& TimerUi_colon = W->CreateGameObject();

    GameObject& ts = W->CreateGameObject();
    CanvasUI& tsUI = ts.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::Number));

    GameObject& os = W->CreateGameObject();
    CanvasUI& osUI = os.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::Number));

    GameObject& TimerUi_manager = W->CreateGameObject();
    TimerUi_manager.AddComponent<TimerUIManager>(&tmUI, &omUI, &tsUI, &osUI);
}

void SceneGame::CreateScoreUi()
{
    // スコアの初期化
    ScoreManager::Instance().Reset(500);

    // 千の位
    GameObject& th = W->CreateGameObject();
    CanvasUI& thUI = th.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::Number));

    // 百の位
    GameObject& h = W->CreateGameObject();
    CanvasUI& hUI = h.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::Number));

    // 十の位
    GameObject& t = W->CreateGameObject();
    CanvasUI& tUI = t.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::Number));

    // 一の位
    GameObject& o = W->CreateGameObject();
    CanvasUI& oUI = o.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::Number));

    GameObject& scoreUiManagerObj = W->CreateGameObject();
    scoreUiManagerObj.AddComponent<ScoreUIManager>(&thUI, &hUI, &tUI, &oUI);
}

void SceneGame::CreateActionUi()
{
    //
    GameObject& spin = W->CreateGameObject();
    CanvasUI& spinUI = spin.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::SpinUI));

    // 
    GameObject& dash = W->CreateGameObject();
    CanvasUI& dashUI = dash.AddComponent<CanvasUI>(TextureManager::Get(TextureTag::DashUI));

    GameObject& actionUiManagerObj = W->CreateGameObject();
    actionUiManagerObj.AddComponent<ActionUIManager>(&spinUI, &dashUI);
}
