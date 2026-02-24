#include "SceneStageSelect.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "SceneManager.h"
#include "InputKeyboard.h"
#include "SceneOption.h"
#include "stageLevel.h"
#include "StageSentaku.h"
#include "StageClearRank.h"

void SceneStageSelect::Load()
{
	// 従来の同期ロードが必要なら、タスクを構築して全部回す
	std::vector<LoadTask> tasks;
	BuildLoadTasks(tasks);
	for (auto& t : tasks) t();
}

void SceneStageSelect::BuildLoadTasks(std::vector<LoadTask>& outTasks)
{

	// World を初期化
	outTasks.emplace_back([this]
		{
			GetWorld().ClearAll();        // 既存の World API に合わせる
		});

	// モデルロード
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Player); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Hammer_1); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Hammer_2); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Hammer_3); });

	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Swing); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Gomibukuro); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Benti); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Aircon); });
	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Ufo); });

	outTasks.emplace_back([this] { ModelManager::Load(ModelTag::Fence); });

	// テクスチャーロード	 
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::StageSelect_BG); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::StageSelect_PlayerIcon); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::stageLv1); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::stageLv2); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::stageLv3); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::stagelevel_Easy); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::stagelevel_Normal); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Stagelevel_Hard_a); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Stagelevel_Hard_b); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Stagelevel_Hard_c); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::StageImage); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Number); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Number2); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Number3); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::bou); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::bou2); });
	// ゲームオブジェクト生成
	outTasks.emplace_back([this] { CreateCanvas(); });
	outTasks.emplace_back([this] { CreateStageMap(); });
	outTasks.emplace_back([this] { CreateCamera(); });
	outTasks.emplace_back([this] { CreatePlayer(); });
	outTasks.emplace_back([this] { CreateGround(); });
	outTasks.emplace_back([this] { CreateStageLevelUI(); });
	outTasks.emplace_back([this] { CreateStageImageUI(); });
	outTasks.emplace_back([this] { CreateStageClearRank(); });

	outTasks.emplace_back([this] { SceneManager::CreateFadeAnimations(this); });

	// 最後に Start を流す
	outTasks.emplace_back([this]
		{
			GetWorld().Start();
		});
}

void SceneStageSelect::CreateCanvas()
{
	World& world = GetWorld();

	GameObject& canvasObj = world.CreateGameObject();
	canvasObj.AddComponent<Canvas>();
}

void SceneStageSelect::CreateStageMap()
{
	World& world = GetWorld();

	GameObject& stageMapObj = world.CreateGameObject();
	stageMapObj.SetTag(Tag::StageMap);
	stageMapObj.AddComponent<StageMap>();
}

void SceneStageSelect::CreateCamera()
{
	World& world = GetWorld();
	GameObject& cameraObj = world.CreateGameObject(Vector3(0.f, 10.f, 0.f), Vector3(90.f, 0.f, 0.f));
	cameraObj.SetTag(Tag::MainCamera);
	cameraObj.AddComponent<Camera>();
}

void SceneStageSelect::CreatePlayer()
{
	World& world = GetWorld();

	GameObject& playerObj = world.CreateGameObject(Vector3(-5.f, 0.f, 0.f));
	playerObj.SetTag(Tag::Player);

	playerObj.AddComponent<MeshFilter>(ModelTag::Player);
	playerObj.AddComponent<MeshRenderer>();
	playerObj.AddComponent<SelectPlayerControl>();
	
}

void SceneStageSelect::CreateGround()
{
	World& world = GetWorld();

	GameObject& groundObj = world.CreateGameObject(Vector3::zero(), Vector3(90.f, 0.f, 0.f));
	groundObj.AddComponent<Sprite3DRenderer>(TextureTag::StageSelect_BG, Vector2::zero(), Vector2(16.f, 9.f));
}

void SceneStageSelect::CreateStageLevelUI()
{
	World& world = GetWorld();

	GameObject& stageLevelUIObj = world.CreateGameObject();
	stageLevelUIObj.SetTag(Tag::StageLevelUI);
	stageLevelUIObj.AddComponent<stageLevel>();
}

void SceneStageSelect::CreateStageImageUI()
{
	World& world = GetWorld();

	GameObject& stageImageUIObj = world.CreateGameObject();
	stageImageUIObj.SetTag(Tag::StagesenntakuUI);
	stageImageUIObj.AddComponent<StageSentaku>();
}

void SceneStageSelect::CreateStageClearRank()
{
	World& world = GetWorld();

	GameObject& StageClearRankUIObj = world.CreateGameObject();
	StageClearRankUIObj.SetTag(Tag::StageClearRank);
	StageClearRankUIObj.AddComponent<StageClearRank>();
}


void SceneStageSelect::Unload()
{
	SceneManager::ReleaseFadeAnimations();
	TextureManager::Clear();
}


void SceneStageSelect::Update()
{
}

void SceneStageSelect::Draw()
{
}

