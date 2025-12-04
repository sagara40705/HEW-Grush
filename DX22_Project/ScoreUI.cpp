#include "ScoreUI.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"


constexpr Vector2 kNumberUVScale = { 0.2f,0.5f };

void ScoreUI::Start()
{
	GameObject* scoreMgr = gameObject->world->FindWithTag(Tag::ScoreManager);
	score_ = scoreMgr->GetComponent<ScoreManager>();

	constexpr Vector2 kNumberPos[ToIndex(ScoreDigits::Count)] = { {100.f,0.f},{140.f,0.f},{180.f,0.f} };
	constexpr Vector2 kNumberSize = { 40.f,60.f };
	for (size_t digits = 0; digits < ToIndex(ScoreDigits::Count); ++digits)
	{
		GameObject& go = gameObject->CreateChild();
		ui_[digits] = &(go.AddComponent<UiSpriteRenderer>(TextureTag::ScoreNumber, kNumberPos[digits], kNumberSize));
		ui_[digits]->SetUVScale(kNumberUVScale);
	}

	constexpr Vector2 kAtoKgPos = { 0.f,0.f };
	constexpr Vector2 kAtoKgSize = { 400.f,80.f };

	GameObject& atoKg = gameObject->CreateChild();
	uiAtoKg_ = &(atoKg.AddComponent<UiSpriteRenderer>(TextureTag::ScoreAtoKg, kAtoKgPos, kAtoKgSize));
}

void ScoreUI::Update()
{
	if (!score_) return;
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
