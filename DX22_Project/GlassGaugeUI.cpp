#include "GlassGaugeUI.h"
#include "GameObject.h"
#include "GlassGaugeManager.h"
#include "UiSpriteRenderer.h"
#include "SpriteAnimation.h"
#include "DebugLog.h"

void GlassGaugeUI::Awake()
{
	GameObject& gaugeEmpty = gameObject->CreateChild();
	empty_ = &gaugeEmpty.AddComponent<UiSpriteRenderer>(TextureTag::GaugeEmpty, Vector2::zero(), Vector2(250,250));

	GameObject& gaugeMax = gameObject->CreateChild();
	max_ = &gaugeMax.AddComponent<UiSpriteRenderer>(TextureTag::GaugeMax , Vector2::zero(), Vector2(250, 250));

	GameObject& gaugeEffect = gameObject->CreateChild();
	gaugeEffect.AddComponent<UiSpriteRenderer>(TextureTag::GaugeEffect, Vector2::zero(), Vector2(250, 250));
	effect_ = &gaugeEffect.AddComponent<SpriteAnimation>(8, 4, 3, 32, false);
}

void GlassGaugeUI::Start()
{
	GameObject* galssGaugeMgr = gameObject->world->FindWithTag(Tag::GaugeManager);
	manager_ = galssGaugeMgr->GetComponent<GlassGaugeManager>();
}

void GlassGaugeUI::Update()
{
	if (!manager_->dirty_) return;

	if (manager_->up_)
	{
		DebugLog::Output("GlassGauge Effect Play");
		effect_->Play();
	}

	float r = manager_->GetGaugeRatio();
	DebugLog::OutputValue("GlassGauge Ratio", r);

	max_->SetOffset({ 0.f, 250.f * (1 - r) });
	max_->SetSize({ 250.f, 250.f * r});
	max_->SetUVPos({ 0, 1 - r });
	max_->SetUVScale({ 1, r });
}

void GlassGaugeUI::OnDestroy()
{
	manager_ = nullptr;
	empty_ = nullptr;
	max_ = nullptr;
	effect_ = nullptr;
}
