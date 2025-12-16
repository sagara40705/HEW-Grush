#include "ScoreUI.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"


constexpr Vector2 kNumberUVScale = { 0.1f,1.0f };

void ScoreUI::Start()
{
	GameObject* scoreMgr = gameObject->world->FindWithTag(Tag::ScoreManager);
	score_ = scoreMgr->GetComponent<ScoreManager>();

	constexpr Vector2 kNumberPos[ToIndex(ScoreDigits::Count)] = { {504.f,40.f},{572.f,40.f},{646.f,40.f} };
	constexpr Vector2 kNumberSize = { 100.f,100.f };
	for (size_t digits = 0; digits < ToIndex(ScoreDigits::Count); ++digits)
	{
		GameObject& go = gameObject->CreateChild();
		ui_[digits] = &(go.AddComponent<UiSpriteRenderer>(TextureTag::ScoreNumber, kNumberPos[digits], kNumberSize));
		ui_[digits]->SetUVScale(kNumberUVScale);
	}

	constexpr Vector2 kAtoKgPos = { 460.f,85.f };
	constexpr Vector2 kAtoKgSize = { 355.f,45.f };

	GameObject& atoKg = gameObject->CreateChild();
	uiAtoKg_ = &(atoKg.AddComponent<UiSpriteRenderer>(TextureTag::ScoreAtoKg, kAtoKgPos, kAtoKgSize));
}

void ScoreUI::Update()
{
	if (!score_) return;

	if (!score_->GetDirty()) return;

	int sc = score_->GetScore();

	ui_[ToIndex(ScoreDigits::One)]->SetUVPosX((sc / 100) % 10 * 0.1);
	ui_[ToIndex(ScoreDigits::Ten)]->SetUVPosX((sc / 10) % 10 * 0.1);
	ui_[ToIndex(ScoreDigits::Hundred)]->SetUVPosX(sc % 10 * 0.1);
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
