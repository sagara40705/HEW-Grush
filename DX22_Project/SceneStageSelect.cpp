#include "SceneStageSelect.h"
#include "GameObject.h"
#include "AllComponent.h"

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

	// テクスチャーロード	

	// ゲームオブジェクト生成
	outTasks.emplace_back([this] { CreateStageMap(); });
	outTasks.emplace_back([this] { CreateCamera(); });
	outTasks.emplace_back([this] { CreatePlayer(); });
	outTasks.emplace_back([this] { CreateGround(); });

	// 最後に Start を流す
	outTasks.emplace_back([this]
		{
			GetWorld().Start();
		});
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

}

void SceneStageSelect::CreatePlayer()
{
	World& world = GetWorld();

	GameObject& playerObj = world.CreateGameObject();
	playerObj.SetTag(Tag::Player);
	playerObj.AddComponent<SelectPlayerControl>();
}

void SceneStageSelect::CreateGround()
{
}


void SceneStageSelect::Unload()
{
	TextureManager::Clear();
	ModelManager::Clear();
}


void SceneStageSelect::Update()
{
}

void SceneStageSelect::Draw()
{
}

