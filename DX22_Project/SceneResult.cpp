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
#include  "ResultManeger.h"
#include "ResultArubauto.h"

//constexpr Vector2 kNumberUVScale = { 0.1f, 1.0f };
void SceneResult::Load()
{

	//テクスチャーロード
	//TextureManager::Load(TextureTag::Title_BG);
	World& w = GetWorld();
	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();



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



	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::TimerColon02); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ResultTime); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Frame); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Gauge); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ResultUI); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::RANKS); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ResultTimer); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::tatu_Arubaita); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::ResultHaikei); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::HaikeiBase); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::pika_Arubaita); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::sugo_Arubaita); });
	outTasks.emplace_back([this] { TextureManager::Load(TextureTag::Next); });
	// 各種オブジェクト生成タスクを追加
	outTasks.emplace_back([this] { CanvasDraw(); });
	outTasks.emplace_back([this] { TimerUIDeaw(); });
	//<<<<<<< HEAD
	outTasks.emplace_back([this] { GaugeDraw(); });
	outTasks.emplace_back([this] { RankDraw(); });
	outTasks.emplace_back([this] { SceneTitleDraw(); });
	outTasks.emplace_back([this] { ArubaitoLevelDraw(); });
	//=======
		//outTasks.emplace_back([this] { GaugeDraw(); });
		//outTasks.emplace_back([this] { RankDraw(); });
	//>>>>>>> e348d9a321fd84f4a3ac8ea23cc62e58cc620fd9

		// 最後に Start を流す
	outTasks.emplace_back([this]
		{
			GetWorld().Start();
		});

}


void SceneResult::Update()
{
}

void SceneResult::Draw()
{
}

void SceneResult::Unload()
{
}

void SceneResult::TimerUIDeaw()
{
	World& w = GetWorld();

	GameObject& timerUI = w.CreateGameObject();
	timerUI.SetTag(Tag::ResultTimerUI);
	timerUI.AddComponent<ResultTimerUI>();

}

void SceneResult::GaugeDraw()
{
	World& w = GetWorld();

	GameObject& Gauge = w.CreateGameObject();
	Gauge.SetTag(Tag::ResultGaugeUI);
	Gauge.AddComponent<ResultGaugeUI>();
}

void SceneResult::CanvasDraw()
{
	World& w = GetWorld();
	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();
	//背景生成

	GameObject& haikeiGroung = w.CreateGameObject();
	haikeiGroung.AddComponent<UiSpriteRenderer>(TextureTag::ResultHaikei, Vector2(0.f, 0.f), Vector2(1280, 780));

	GameObject& BaseGroung = w.CreateGameObject();
	BaseGroung.AddComponent<UiSpriteRenderer>(TextureTag::HaikeiBase, Vector2(0.f, 0.f), Vector2(1280, 780));

	/*GameObject& backGroung = w.CreateGameObject();
	backGroung.AddComponent<UiSpriteRenderer>(TextureTag::ResultUI, Vector2(550.f, -50.f), Vector2(600, 300));*/
	GameObject& timeGroung = w.CreateGameObject();
	timeGroung.AddComponent<UiSpriteRenderer>(TextureTag::ResultTime, Vector2(0.f, 0.f), Vector2(1280, 780));


	GameObject& NextGroung = w.CreateGameObject();
	NextGroung.AddComponent<UiSpriteRenderer>(TextureTag::Next, Vector2(0.f, 0.f), Vector2(1280, 750));
	//GameObject& tatuGroung = w.CreateGameObject();
	//tatuGroung.AddComponent<UiSpriteRenderer>(TextureTag::tatu_Arubaita, Vector2(700.f, 180.f), Vector2(420, 100));
}

void SceneResult::RankDraw()
{
	World& w = GetWorld();

	GameObject& Rank = w.CreateGameObject();
	Rank.SetTag(Tag::ResultRankUI);
	Rank.AddComponent<ResultRankUI>();
}

void SceneResult::SceneTitleDraw()
{
	World& w = GetWorld();
	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();
	//背景生成
	//タイトルマネージャー生成
	GameObject& manager = w.CreateGameObject();
	manager.AddComponent<ResultManeger>();
}

void SceneResult::ArubaitoLevelDraw()
{
	World& w = GetWorld();

	GameObject& Arubaito = w.CreateGameObject();
	Arubaito.SetTag(Tag::ResultArubaito);
	Arubaito.AddComponent<ResultArubauto>();
}

