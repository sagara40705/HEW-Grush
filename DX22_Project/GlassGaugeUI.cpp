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
	empty_ = &gaugeEmpty.AddComponent<UiSpriteRenderer>(TextureTag::GaugeEmpty, Vector2({ 25.f,10.f }) , Vector2(220, 200));

	GameObject& gaugeEm = gameObject->CreateChild();
	em_ = &gaugeEm.AddComponent<UiSpriteRenderer>(TextureTag::GaugeFlash, Vector2({ 25.f,10.f }), Vector2(220, 200));

	GameObject& gaugeMax = gameObject->CreateChild();
	max_ = &gaugeMax.AddComponent<UiSpriteRenderer>(TextureTag::GaugeMax , Vector2({ 25.f,10.f }), Vector2(220, 200));

	GameObject& gaugeEffect = gameObject->CreateChild();
	gaugeEffect.AddComponent<UiSpriteRenderer>(TextureTag::GaugeEffect, Vector2({30.f,-100.f}), Vector2(192, 384));
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

	// 減衰が始まった瞬間にタイマーをリセット
	static bool wasDecaying = false;
	bool isNowDecaying = manager_->IsGaugeDecaying();
	if (isNowDecaying && !wasDecaying)
	{
		decayTimer = gDecayDuration;
	}
	wasDecaying = isNowDecaying;

	if (manager_->up_)
	{
		DebugLog::Output("GlassGauge Effect Play");
		effect_->Play();
	}

	float r = manager_->GetGaugeRatio();
	float g = manager_->GetGaugeDecayTimer();
	float garde = static_cast<float>(manager_->GetHammerGrade());

	DebugLog::OutputValue("GlassGauge Ratio", r);

	max_->SetOffset({ 25, 200.f * (1 - r) + 10.f });
	max_->SetSize({ 220.f , 200.f * r });
	max_->SetUVPos({ garde / 3.f, 1 - r });
	max_->SetUVScale({ 1.f / 3.f, r });

	em_->SetUVPos({ garde / 3.f, 1 });
	em_->SetUVScale({ 1.f / 3.f, 1 });
	empty_->SetUVPos({ garde / 3.f, 1 });
	empty_->SetUVScale({ 1.f / 3.f, 1 });

	// ゲージ減衰中または
	// ゲージ減少までのタイマーの値が一定以上のとき点滅させる
	if (manager_->IsGaugeDecaying() || g > 3.0f)
	{
		decayTimer -= 1.0f / 60.0f;

		float t = decayTimer / gDecayDuration; // 1.0 → 0.0

		float brightness = 0.5f + 0.5f * std::sin(t * 3.14f); // 0.0〜1.0

		// 明るさを調整した黄色（RとGを変える）
		float r = brightness;
		float g = brightness;
		float b = 0.0f;

		empty_->SetColor({ 1.f,1.f,1.f,1.f });
		em_->SetColor({ r, g, b, 1.0f }); // アルファは固定
	}
	else
	{
		em_->SetColor({ 1.f,1.f,1.f, 1.f }); // 元に戻す
	}

}

void GlassGaugeUI::OnDestroy()
{
	manager_ = nullptr;
	em_ = nullptr;
	empty_ = nullptr;
	max_ = nullptr;
	effect_ = nullptr;
}
