#include "ScoreUI.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

constexpr Vector2 kNumberUVScale = { 0.2f,0.5f };

void ScoreUI::Awake()
{
	constexpr Vector2 kNumberPos[ToIndex(ScoreDigits::Count)] = { {515.f,22.f},{575.f,22.f},{635.f,22.f},{695.f,22.f} };
	constexpr Vector2 kNumberSize = { 105.f,105.f };
	for (size_t digits = 0; digits < ToIndex(ScoreDigits::Count); ++digits)
	{
		GameObject& go = gameObject->CreateChild();
		ui_[digits] = &(go.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Score_Number, kNumberPos[digits], kNumberSize));
		ui_[digits]->SetUVScale(kNumberUVScale);
	}

	constexpr Vector2 kAtoKgPos = { 463.f,68.f };
	constexpr Vector2 kAtoKgSize = { 410.f,55.f };

	GameObject& atoKg = gameObject->CreateChild();
	uiAtoKg_ = &(atoKg.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Score_Text, kAtoKgPos, kAtoKgSize));
}

void ScoreUI::Start()
{
	GameObject* scoreMgr = gameObject->world->FindWithTag(Tag::ScoreManager);
	score_ = scoreMgr->GetComponent<ScoreManager>();
}

void ScoreUI::Update()
{
	if (!score_) return;

	if (!score_->GetDirty()) return;

	int sc = score_->GetScore();
	int digitsValues[4] = {
		(sc / 1000) % 10,		// êÁ
		(sc / 100) % 10,        // ïS
		(sc / 10) % 10,         // è\
		(sc % 10)               // àÍ
	};

	const float cellW = 0.2f; // 1.0 / 5óÒ
	const float cellH = 0.5f; // 1.0 / 2çs

	for (int i = 0; i < 4; ++i)
	{
		int num = digitsValues[i];

		float u = (num % 5) * cellW;
		float v = (num / 5) * cellH;

		ui_[i]->SetUVPos({ u, v });
		ui_[i]->SetUVScale(kNumberUVScale);
	}
}

void ScoreUI::OnDestroy()
{
	uiAtoKg_ = nullptr;
	for (size_t digits = 0; digits < ToIndex(ScoreDigits::Count); ++digits)
	{
		ui_[digits] = nullptr;
	}
	score_ = nullptr;
}
