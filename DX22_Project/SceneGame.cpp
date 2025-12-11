#include "SceneGame.h"
#include <DirectXMath.h>
#include <fstream>  // ファイル読み込み用
#include <sstream>  // 文字列分割用
#include <vector>   // データ格納用
#include <string>   // 文字列操作用
#include "Defines.h"
#include "AllComponet.h"
#include "PhysicsSystem.h"
#include "SampleMapData.h"
#include "Model.h"
#include "LineManager.h"
#include "RenderSystem.h"
#include "Game.h"
#include "GameObject.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Prefab.h"
#include <direct.h>
#include <algorithm>

World* W = nullptr;

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

SceneGame::SceneGame(){}
SceneGame::~SceneGame(){}

void SceneGame::Load()
{
    W = &GetWorld();

    Debug::Init();
    RegisterTexture();
    RegisterModel();
    RegisterAllPrefabs();

    SetPlayerAndHummer();
    LineManager::Init();

    GameObject& floor = W->CreateGameObject();
    floor.AddComponent<Transform>(Vector3(0, -1, 0), Vector3::zero(), Vector3(50, 0.1, 50));
    floor.AddComponent<MeshFilter>(MeshFilter::Type::Plane);
    floor.AddComponent<MeshRender>();
    floor.AddComponent<BoxCollider>();
    floor.SetTag(Tag::Ground);

    //BuildBlocksFromMap(map, /*unit=*/kUnit, /*scale=*/blockScale, /*isStaticCollider=*/true);

    GameObject& karubi = W->CreateGameObject();
    karubi.AddComponent<Transform>(DirectX::XMFLOAT3{ 2,0.5,2 }, Vec3Zero, Vec3One);
    karubi.AddComponent<ModelComponent>(ModelManager::Get(ModelTag::Player));
    karubi.AddComponent<ModelRenderer>();
    karubi.AddComponent<BoxCollider>(true, DirectX::XMFLOAT3{ 0,0.75f,0 }, DirectX::XMFLOAT3{ 1, 1.5, 1 });
    LineManager::AddBoxCollider(&karubi);

    //CreateProtStage();
    CreateStageFromCSV("StageData.csv");
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
    gaugeUiManagerObj.AddComponent<GlassGauge>(&gaugeFrameUI, &gaugeUI, &gaefUI);
}
void SceneGame::Update()
{
}
void SceneGame::Draw()
{
}

void SceneGame::Unload()
{
    W = nullptr;
    TextureManager::Clear();
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
    ModelManager::RegisterAuto(ModelTag::Player);
}

void SceneGame::CreateProtStage()
{

   /* Prefab::Instantiate(*W, PrefabId::Ufo, Vector3(10, -0.5f, 10), Vector3::zero(), Vector3::one());

    Prefab::Instantiate(*W, PrefabId::Benti, Vector3(-10, -0.9f, 10), Vector3::zero(), Vector3(1.2f, 1.2f, 1.2f));
    Prefab::Instantiate(*W, PrefabId::Benti, Vector3(20, -0.9f, -16), Vector3::zero(), Vector3(1.2f, 1.2f, 1.2f));
    Prefab::Instantiate(*W, PrefabId::Benti, Vector3(11, -0.9f, -13), Vector3::zero(), Vector3(1.2f, 1.2f, 1.2f));

    Prefab::Instantiate(*W, PrefabId::Swing, Vector3(-7, -0.9f, -8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::Swing, Vector3(-12, -0.9f, -2), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::Swing, Vector3(-22, -0.9f, -13), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));

    Prefab::Instantiate(*W, PrefabId::ProtPlay, Vector3(-12, -0.9f, 8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::ProtPlay, Vector3(5, -0.9f, 2), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::ProtPlay, Vector3(-20, -0.9f, -20), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::ProtPlay, Vector3(-16, -0.9f, -16), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::ProtPlay, Vector3(13, -0.9f, 2), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::ProtPlay, Vector3(22, -0.9f, 8), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::ProtPlay, Vector3(-17, -0.9f, 15), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::ProtPlay, Vector3(6, -0.9f, 17), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::ProtPlay, Vector3(20, -0.9f, -12), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));
    Prefab::Instantiate(*W, PrefabId::ProtPlay, Vector3(14, -0.9f, -19), Vector3::zero(), Vector3(0.8f, 0.8f, 0.8f));*/
}

std::string CleanString(std::string str)
{  // 文字列から " とスペースを削除するヘルパー関数
    str.erase(std::remove(str.begin(), str.end(), '\"'), str.end());
    return str;
}


void SceneGame::CreateStageFromCSV(const std::string& filename)
{
    /*char path[1024];
    if (_getcwd(path, 1024) != NULL) 
    {
        std::string fullPath = std::string(path) + "\\" + filename;
        OutputDebugStringA("\n========================================\n");
        OutputDebugStringA("【確認してください】\n");
        OutputDebugStringA("プログラムはこの場所にあるファイルを探しています:\n");
        OutputDebugStringA(fullPath.c_str());
        OutputDebugStringA("\n========================================\n");
    }*/

    /*std::string debugMsg = "Current Directory: " + std::string(path) + "\n";
    OutputDebugStringA(debugMsg.c_str());*/

    std::ifstream file(filename);
    if (!file.is_open())
    {   // ファイルが見つからない場合のエラー処理
        OutputDebugStringA("Error: CSVファイルが見つかりません！\n");
        return;
    }

    OutputDebugStringA("Success: CSVファイルを開きました！読み込みを開始します...\n");

    std::string line;
    // ファイルを1行ずつ読み込む
    while (std::getline(file, line))
    {   
        //if (line.empty() || line.length() < 3)continue;
        //// コメント行スキップ
        //if (line[0] == '#') continue;
        
        // '#'で始まる行はコメントとしてスキップ
        if (line.empty() || line[0] == '#') continue;

        std::stringstream ss(line);
        std::string segment;
        std::vector < std::string > data;

        // カンマ区切りで分解
        while (std::getline(ss, segment, ','))
        {
            data.push_back(CleanString(segment));
        }

        // データが7つ(ID, PosXYZ, ScaleXYZ)揃っているか確認
        if (data.size() >= 7)
        { 
            try 
            {
                // クリーンなデータを使って変換
                // 文字列を数値に変換
                int id = std::stoi(data[0]);

               /* std::string log = "Loaded ID: " + std::to_string(id) + "\n";
                OutputDebugStringA(log.c_str());*/
                float px = std::stof(data[1]);
                float py = std::stof(data[2]);
                float pz = std::stof(data[3]);
                float rx = std::stof(data[4]);
                float ry = std::stof(data[5]);
                float rz = std::stof(data[6]);

                // Prefabを生成
                Prefab::Instantiate(*W, static_cast<PrefabId>(id),
                    Vector3(px, py, pz), // 位置
                    Vector3(rx, ry, rz), // 回転
                    Vector3(1, 1, 1)     // スケール
                );
            }
            catch (const std::exception& e)
            {
                std::string errorMsg = "Error: 変換失敗！ Reason: " + std::string(e.what()) + "\n";
                if (!data.empty()) errorMsg += "  Bad Data: [" + data[0] + "]\n";

                OutputDebugStringA(errorMsg.c_str());
            }
        }
    }
    file.close();
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
    GameObject& camera = W->CreateGameObject();
    Transform& cameraTf = camera.AddComponent<Transform>(Camera::initPos(), Camera::initRotate(), Vector3::one());
    camera.AddComponent<Camera>();
    camera.AddComponent<CameraShake>();

    GameObject& pivot = W->CreateGameObject();
    Transform& pivotTf = pivot.AddComponent<Transform>(Vector3(0,1,0), Vector3::zero(), Vector3::one());
    pivot.AddComponent<CameraControl>();

    GameObject& player = W->CreateGameObject();
    Transform& playerTf = player.AddComponent<Transform>(Vector3(0, 2, 0), Vector3::zero(), Vector3(1, 1, 1));
    player.AddComponent<MeshFilter>(MeshFilter::Type::Cylinder);
    player.AddComponent<MeshRender>(1);
    player.AddComponent<BoxCollider>(true, Vector3(0, 0, 0), Vector3(1, 1, 1));
    player.AddComponent<Rigidbody>();
    player.AddComponent<PlayerMove>();
    player.AddComponent<PlayerAction>();
    player.SetTag(Tag::Player);
  
    GameObject& hammer = W->CreateGameObject();
    Transform& hammerTf = hammer.AddComponent<Transform>(Vector3(0, 0, 1.5f), Vector3::zero(), kDefaultHummerScale);
    hammer.AddComponent<MeshFilter>(MeshFilter::Type::Cube);
    hammer.AddComponent<MeshRender>(1);
    auto& hbc = hammer.AddComponent<BoxCollider>(false); hbc.isTrigger = true;
    hammer.AddComponent<HummerScript>(&playerTf);
    hammer.SetTag(Tag::Weapon);

    hammerTf.SetParent(&playerTf);
    cameraTf.SetParent(&pivotTf);
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
