#include "GlassGaugeUI.h"
#include "GameObject.h"
#include "GlassGaugeManager.h"
#include "UiSpriteRenderer.h"
#include "SpriteAnimation.h"
#include "DebugLog.h"
#include "Hammer.h"

void GlassGaugeUI::Awake()
{
	GameObject& gaugeEmpty = gameObject->CreateChild();
	empty_ = &gaugeEmpty.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Gauge_Empty, Vector2({ 25.f,10.f }) , Vector2(200, 200));

	GameObject& gaugeMax = gameObject->CreateChild();
	max_ = &gaugeMax.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Gauge_Max, Vector2({ 25.f,10.f }), Vector2(200, 200));

	GameObject& gaugeEffect = gameObject->CreateChild();
	gaugeEffect.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Gauge_UpEffect, Vector2({20.f,40.f}), Vector2(200, 125));
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
	float garde = static_cast<float>(manager_->GetHammerGrade());

	DebugLog::OutputValue("GlassGauge Ratio", r);

	max_->SetOffset({ 25, 200.f * (1 - r) + 10.f });
	max_->SetSize({ 220.f , 200.f * r });
	max_->SetUVPos({ garde / 3.f, 1 - r });
	max_->SetUVScale({ 1.f / 3.f, r });

	empty_->SetOffset({ 25.f, 10.f });
	empty_->SetSize({ 220.f , 200.f  });
	empty_->SetUVPos({ garde / 3.f, 1.f  });
	empty_->SetUVScale({ 1.f / 3.f, 1.f / 2.f});

}

void GlassGaugeUI::OnDestroy()
{
	manager_ = nullptr;
	empty_ = nullptr;
	max_ = nullptr;
	effect_ = nullptr;
}
