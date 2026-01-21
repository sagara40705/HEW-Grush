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
#include "SceneStageSelect.h"


constexpr Vector2 kNumberUVScale = { 0.1f, 1.0f };
void Result::Load()
{
	// 従来の同期ロードが必要なら、タスクを構築して全部回す
	std::vector<LoadTask> tasks;
	BuildLoadTasks(tasks);
	for (auto& t : tasks) t();
}

void Result::BuildLoadTasks(std::vector<LoadTask>& outTasks)
{
	// World の中身を消す
	outTasks.emplace_back([this]
		{
			GetWorld().ClearAll();        // 既存の World API に合わせる
		});

	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::TimerColon); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ResultTime); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Frame); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Gauge); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ResultUI); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::RANKS); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ResultTimer); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::tatu_Arubaita); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ResultHaikei); });

	// 各種オブジェクト生成タスクを追加
	outTasks.emplace_back([this] { CanvasDraw(); });
	outTasks.emplace_back([this] { TimerUIDeaw(); });
	outTasks.emplace_back([this] { GaugeDraw(); });
	outTasks.emplace_back([this] { RankDraw(); });	
	outTasks.emplace_back([this] { SceneManager::CreateFadeAnimations(this); });


		// 最後に Start を流す
	outTasks.emplace_back([this]
		{
			GetWorld().Start();
		});

}

void Result::Update()
{
	if (InputKeyboard::GetKeyDown(VK_SPACE))
	{
		SceneManager::ChangeSceneAsync<SceneStageSelect>();
	}
}

void Result::Draw()
{
}

void Result::Unload()
{
	SceneManager::ReleaseFadeAnimations();
	TextureManager::Clear();
}

void Result::TimerUIDeaw()
{
	World& w = GetWorld();

	GameObject& timerUI = w.CreateGameObject();
	timerUI.SetTag(Tag::ResultTimerUI);
	timerUI.AddComponent<ResultTimerUI>();

}

void Result::GaugeDraw()
{
	World& w = GetWorld();

	GameObject& Gauge = w.CreateGameObject();
	Gauge.SetTag(Tag::ResultGaugeUI);
	Gauge.AddComponent<ResultGaugeUI>();
}

void Result::CanvasDraw()
{
	World& w = GetWorld();
	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();
	//背景生成

	GameObject& haikeiGroung = w.CreateGameObject();
	haikeiGroung.AddComponent<UiSpriteRenderer>(TextureTag::ResultHaikei, Vector2(0.f, 0.f), Vector2(1280, 780));
	GameObject& backGroung = w.CreateGameObject();
	backGroung.AddComponent<UiSpriteRenderer>(TextureTag::ResultUI, Vector2(550.f, -50.f), Vector2(600, 300));
	GameObject& timeGroung = w.CreateGameObject();
	timeGroung.AddComponent<UiSpriteRenderer>(TextureTag::ResultTime, Vector2(800.f, 380.f), Vector2(220, 100));
	GameObject& tatuGroung = w.CreateGameObject();
	tatuGroung.AddComponent<UiSpriteRenderer>(TextureTag::tatu_Arubaita, Vector2(700.f, 180.f), Vector2(420, 100));
}

void Result::RankDraw()
{
	World& w = GetWorld();

	GameObject& Rank = w.CreateGameObject();
	Rank.SetTag(Tag::ResultRankUI);
	Rank.AddComponent<ResultRankUI>();
}