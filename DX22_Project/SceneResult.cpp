#include "SceneResult.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include "InputKeyboard.h"
#include "ResultTimerUI.h"
#include "LineManager.h"
#include "ResultGaugeUI.h"
#include "ResultRankUI.h"
#include "ResultArubaito.h"
#include "SceneStageSelect.h"


constexpr Vector2 kNumberUVScale = { 0.1f, 1.0f };
void SceneResult::Load()
{
	// 従来の同期ロードが必要なら、タスクを構築して全部回す
	std::vector<LoadTask> tasks;
	BuildLoadTasks(tasks);
	for (auto& t : tasks) t();
}

void SceneResult::BuildLoadTasks(std::vector<LoadTask>& outTasks)
{
	// World の中身を消す
	outTasks.emplace_back([this]
		{
			GetWorld().ClearAll();        // 既存の World API に合わせる
		});

	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Bace); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Time_Text); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Time_Number); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Time_Colon); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Gauge_Bar); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Gauge_Frame); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Next_Text); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Rank_S); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Rank_A); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Rank_B); });
	//outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Rank_C); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Job_Pikapika); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Job_Sugoude); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Result_Job_Tastuzin); });


	// 各種オブジェクト生成タスクを追加
	outTasks.emplace_back([this] { CreateCanvas(); });
	outTasks.emplace_back([this] { CreateBace(); });
	outTasks.emplace_back([this] { CreateTimeUI(); });
	outTasks.emplace_back([this] { CreateGaugeUI(); });
	outTasks.emplace_back([this] { CreateRankUI(); });
	outTasks.emplace_back([this] { CreateJobUI(); });
	outTasks.emplace_back([this] { CreateNextText(); });
	outTasks.emplace_back([this] { SceneManager::CreateFadeAnimations(this); });


		// 最後に Start を流す
	outTasks.emplace_back([this]
		{
			GetWorld().Start();
		});

}

void SceneResult::Update()
{
	if (InputKeyboard::GetKeyDown(VK_SPACE))
	{
		SceneManager::ChangeSceneAsync<SceneStageSelect>();
	}
}

void SceneResult::Draw()
{
}

void SceneResult::Unload()
{
	SceneManager::ReleaseFadeAnimations();
	TextureManager::Clear();
}

void SceneResult::CreateCanvas()
{
	World& w = GetWorld();
	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();
}

void SceneResult::CreateBace()
{
	World& w = GetWorld();
	//背景生成
	GameObject& bace = w.CreateGameObject();
	bace.AddComponent<UiSpriteRenderer>(TextureTag::Result_Bace, Vector2::zero(), Vector2::WindowSize());
}

void SceneResult::CreateTimeUI()
{
	World& w = GetWorld();

	GameObject& timerUI = w.CreateGameObject();
	timerUI.AddComponent<ResultTimerUI>();
}

void SceneResult::CreateGaugeUI()
{
	World& w = GetWorld();

	GameObject& Gauge = w.CreateGameObject();
	Gauge.AddComponent<ResultGaugeUI>();
}

void SceneResult::CreateRankUI()
{
	World& w = GetWorld();

	GameObject& Rank = w.CreateGameObject();;
	Rank.SetTag(Tag::ResultRankUI);
	Rank.AddComponent<ResultRankUI>();
}

void SceneResult::CreateJobUI()
{
	World& w = GetWorld();
	//背景生成
	GameObject& job = w.CreateGameObject();;
	job.SetTag(Tag::ResultArubaito);
	job.AddComponent<ResultArubaito>();
}

void SceneResult::CreateNextText()
{
	World& w = GetWorld();
	//背景生成
	GameObject& job = w.CreateGameObject();
	job.AddComponent<UiSpriteRenderer>(TextureTag::Result_Next_Text, Vector2::zero(), Vector2::WindowSize());
}
