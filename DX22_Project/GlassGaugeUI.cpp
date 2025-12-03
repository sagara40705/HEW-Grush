#include "GlassGaugeUI.h"
#include "GameObject.h"
#include "GlassGaugeManager.h"
#include "UiSpriteRenderer.h"
#include "SpriteAnimation.h"

void GlassGaugeUI::Awake()
{
}

void GlassGaugeUI::Start()
{
	// éQè∆éÊìæ
	ggm_ = nullptr;
	ui_ = GetComponent<UiSpriteRenderer>();
	efAnime_ = effect_->GetComponent<SpriteAnimation>();
}

void GlassGaugeUI::Update()
{

}

void GlassGaugeUI::OnDestroy()
{
	ggm_ = nullptr;
	ui_ = nullptr;
	effect_ = nullptr;
	efAnime_ = nullptr;
}
