#include "SceneResult.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "AllComponent.h"
#include "TextureManager.h"
#include "Defines.h"
#include "InputKeyboard.h"
#include "ResultTimerUI.h"
#include "LineManager.h"
#include "Gauge.h"
#include "Rank.h"

constexpr Vector2 kNumberUVScale = { 0.1f, 1.0f };
void Result::Load()
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


	//constexpr Vector2 kMinutePos[ToIndex(TimerDigits::Count)] = { {1115.f, 14.f}, {1140.f, 14.f} };
	//constexpr Vector2 kColonPos = { 1165.f,16.f };
	//constexpr Vector2 kSecondPos[ToIndex(TimerDigits::Count)] = { {1175.f, 14.f}, {1200.f, 14.f} };

	//constexpr Vector2 kNumberSize = { 25.f, 25.f };
	//constexpr Vector2 kColonSize = { 10.f, 20.f };


	//GameObject& tenMinute = gameObject->CreateChild();
	//uiMinute_[ToIndex(TimerDigits::Ten)] = &tenMinute.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kMinutePos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	//GameObject& oneMinute = gameObject->CreateChild();
	//uiMinute_[ToIndex(TimerDigits::One)] = &oneMinute.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kMinutePos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);

	//GameObject& colon = gameObject->CreateChild();
	//uiColon_ = &colon.AddComponent<UiSpriteRenderer>(TextureTag::TimerColon, kColonPos, kColonSize);

	//GameObject& tenSecond = gameObject->CreateChild();
	//uiSecond_[ToIndex(TimerDigits::Ten)] = &tenSecond.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kSecondPos[ToIndex(TimerDigits::Ten)], kNumberSize, Vector2::zero(), kNumberUVScale);

	//GameObject& oneSecond = gameObject->CreateChild();
	//uiSecond_[ToIndex(TimerDigits::One)] = &oneSecond.AddComponent<UiSpriteRenderer>(TextureTag::TimerNumber, kSecondPos[ToIndex(TimerDigits::One)], kNumberSize, Vector2::zero(), kNumberUVScale);
	//// 
	//// 
	//// 
//	timerUI_ = &canvas.AddComponent<UiSpriteRenderer>(TextureTag::Title_BG, Vector2(0.f, 0.f), Vector2(SCREEN_WIDTH, SCREEN_HEIGHT)); 
	//背景生成
	/*GameObject& backGroung = w.CreateGameObject();
	backGroung.AddComponent<UiSpriteRenderer>(TextureTag::Title_BG, Vector2(0.f, 0.f), Vector2(SCREEN_WIDTH, SCREEN_HEIGHT));*/

	//プレイヤー画像生成


	//タイトルマネージャー生成
	//GameObject& manager = w.CreateGameObject();
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



	// 各種オブジェクト生成タスクを追加
	outTasks.emplace_back([this] { CanvasDraw(); });
	outTasks.emplace_back([this] { TimerUIDeaw(); });
	//<<<<<<< HEAD
	outTasks.emplace_back([this] { GaugeDraw(); });
	outTasks.emplace_back([this] { RankDraw(); });
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


void Result::Update()
{
}

void Result::Draw()
{
}

void Result::Unload()
{
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
	Gauge.SetTag(Tag::GaugeUI);
	Gauge.AddComponent<GaugeUI>();
}

void Result::CanvasDraw()
{
	World& w = GetWorld();
	//Canvas
	GameObject& canvas = w.CreateGameObject();
	canvas.AddComponent<Canvas>();
	//背景生成
	GameObject& backGroung = w.CreateGameObject();
	backGroung.AddComponent<UiSpriteRenderer>(TextureTag::ResultUI, Vector2(350.f, -50.f), Vector2(600, 300));
	GameObject& timeGroung = w.CreateGameObject();
	timeGroung.AddComponent<UiSpriteRenderer>(TextureTag::ResultTime, Vector2(600.f, 380.f), Vector2(220, 100));
	GameObject& tatuGroung = w.CreateGameObject();
	tatuGroung.AddComponent<UiSpriteRenderer>(TextureTag::tatu_Arubaita, Vector2(500.f, 180.f), Vector2(420, 100));



}

void Result::RankDraw()
{
	World& w = GetWorld();

	GameObject& Rank = w.CreateGameObject();
	Rank.SetTag(Tag::RankUI);
	Rank.AddComponent<RankUI>();
}