#include "GlassGauge.h"
#include "GameObject.h"
#include "GlassGaugeManager.h"
#include "UiSpriteRenderer.h"
#include "SpriteAnimation.h"

void GlassGaugeUi::Awake()
{
}

void GlassGaugeUi::Start()
{
	// éQè∆éÊìæ
	ggm_ = nullptr;
	ui_ = GetComponent<UiSpriteRenderer>();
	efAnime_ = effect_->GetComponent<SpriteAnimation>();
}

void GlassGaugeUi::Update()
{

}

void GlassGaugeUi::OnDestroy()
{
	ggm_ = nullptr;
	ui_ = nullptr;
	effect_ = nullptr;
	efAnime_ = nullptr;
}
