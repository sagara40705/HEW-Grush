#include "GlassGaugeUI.h"
#include "GameObject.h"
#include "GlassGaugeManager.h"
#include "UiSpriteRenderer.h"
#include "SpriteAnimation.h"
#include "DebugLog.h"
#include "Hammer.h"
void GlassGaugeUI::Awake()
{
	
	GameObject& gaugeEm = gameObject->CreateChild();
	em_ = &gaugeEm.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Gauge_Empty, Vector2({ 25.f,10.f }), Vector2(220, 200));

	GameObject& gaugeMax = gameObject->CreateChild();
	max_ = &gaugeMax.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Gaug_Lv1, Vector2({ 25.f,10.f }), Vector2(220, 200));

	GameObject& gaugeEffect = gameObject->CreateChild();
	gaugeEffect.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Gauge_UpEffect, Vector2({ 30.f, 0.f }), Vector2(270, 180));
	effect_ = &gaugeEffect.AddComponent<SpriteAnimation>(6, 4, 3, 24, false);

	GameObject& feverGuage= gameObject->CreateChild();
	fever_ = &feverGuage.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Gauge_Fever, Vector2({ 25.f,10.f }), Vector2(220, 200));
	fever_->SetEnabled(false);
}

void GlassGaugeUI::Start()
{
	GameObject* galssGaugeMgr = gameObject->world->FindWithTag(Tag::GaugeManager);
	manager_ = galssGaugeMgr->GetComponent<GlassGaugeManager>();
}

void GlassGaugeUI::Update()
{
	if (manager_->changeLimitBreak)
	{
		fever_->SetEnabled(manager_->GetIsLimitBreak());
		em_->SetEnabled(!manager_->GetIsLimitBreak());
		max_->SetEnabled(!manager_->GetIsLimitBreak());
		manager_->changeLimitBreak = false;
	}

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
	
	float r = manager_->GetGaugeRatio(); // 0.0 ~ 1.0
	float g = manager_->GetGaugeDecayTimer();
	static int lastGrade = -1;
	int grade = static_cast<float>(manager_->GetHammerGrade());
	// グレードに応じたテクスチャタグの配列

	if (grade != lastGrade)
	{
		TextureTag tags[] = {
			TextureTag::Game_UI_Gaug_Lv1,
			TextureTag::Game_UI_Gaug_Lv2,
			TextureTag::Game_UI_Gaug_Lv3
		};
		max_->SetTexture(tags[grade]);
		lastGrade = grade;
	}

	DebugLog::OutputValue("GlassGauge Ratio", r);

	// --- スプライトシートの設定 (5x5) ---
	const int COLUMNS = 5;      // 横に5列
	const int ROWS = 5;         // 縦に5行
	const int TOTAL_FRAMES = 21; 

	//割合(r)を0〜20のインデックスに変換
	// $index = \lfloor r \times 20 + 0.5 \rfloor$
	int frameIndex = static_cast<int>(r * (TOTAL_FRAMES - 1) + 0.5f);

	// インデックスを列(X)と行(Y)に分解する
	int col = frameIndex % COLUMNS;
	int row = frameIndex / COLUMNS;

	//1コマあたりのUVサイズを計算
	float cellW = 1.0f / (float)COLUMNS; // 0.2
	float cellH = 1.0f / (float)ROWS;    // 0.2

	// --- UVの設定 ---
	// 伸び縮みさせないのでScaleは固定(0.2, 0.2)
	max_->SetUVScale({ cellW, cellH });
	// 計算した列・行からUVの左上座標を指定
	max_->SetUVPos({ (float)col * cellW, (float)row * cellH });

	max_->SetOffset({ 25,10.f });
	max_->SetSize({ 220.f , 200.f });


	em_->SetUVPos({ grade / 3.f, 1 });
	em_->SetUVScale({ 1.f / 3.f, 1 });
	/*empty_->SetUVPos({ garde / 3.f, 1 });
	empty_->SetUVScale({ 1.f / 3.f, 1 });*/

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

		//empty_->SetColor({ 1.f,1.f,1.f,1.f });
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
	//empty_ = nullptr;
	max_ = nullptr;
	effect_ = nullptr;
}

