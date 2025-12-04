#include "ScoreUI.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"

constexpr Vector2 kNumberPos[ToIndex(ScoreDigits::Count)] = { {40.f,60.f},{40.f,60.f},{40.f,60.f} };
constexpr Vector2 kNumberSize = { 40.f,60.f };
constexpr Vector2 kNumberUVScale = { 0.2f,0.5f };

void ScoreUI::Start()
{
	GameObject* scoreMgr = gameObject->world->FindWithTag(Tag::ScoreManager);
	score_ = scoreMgr->GetComponent<ScoreManager>();

	for (size_t digits = 0; digits < ToIndex(ScoreDigits::Count); ++digits)
	{
		GameObject& go = gameObject->CreateChild();
		ui_[digits] = &(go.AddComponent<UiSpriteRenderer>(TextureTag::Star, kNumberPos[digits], kNumberSize));
		ui_[digits]->SetUV(GetUVPos(0), kNumberUVScale);
	}
}

void ScoreUI::Update()
{

}

void ScoreUI::OnDestroy()
{
	for (size_t digits = 0; digits < ToIndex(ScoreDigits::Count); ++digits)
	{
		ui_[digits] = nullptr;
	}
	score_ = nullptr;
}

Vector2 ScoreUI::GetUVPos(int n)
{
	return Vector2( n % 5, n / 5 ) * kNumberUVScale;
}
