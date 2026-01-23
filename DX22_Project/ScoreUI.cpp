#include "ScoreUI.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

constexpr Vector2 kNumberUVScale = { 0.1f,1.0f };

void ScoreUI::Awake()
{
	constexpr Vector2 kNumberPos[ToIndex(ScoreDigits::Count)] = { {525.f,40.f},{585.f,40.f},{645.f,40.f},{705.f,40.f} };
	constexpr Vector2 kNumberSize = { 77.f,77.f };
	for (size_t digits = 0; digits < ToIndex(ScoreDigits::Count); ++digits)
	{
		GameObject& go = gameObject->CreateChild();
		ui_[digits] = &(go.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Score_Number, kNumberPos[digits], kNumberSize));
		ui_[digits]->SetUVScale(kNumberUVScale);
	}

	constexpr Vector2 kAtoKgPos = { 960.f,85.f };
	constexpr Vector2 kAtoKgSize = { 300.f,40.f };

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

	ui_[ToIndex(ScoreDigits::One)]->SetUVPosX(((sc / 100) % 10) * 0.1f);
	ui_[ToIndex(ScoreDigits::Ten)]->SetUVPosX(((sc / 10) % 10) * 0.1f);
	ui_[ToIndex(ScoreDigits::Hundred)]->SetUVPosX((sc % 10) * 0.1f);
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
