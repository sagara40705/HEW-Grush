#include "ScoreUI.h"
#include "GameObject.h"
#include "ScoreManager.h"
#include "UiSpriteRenderer.h"
#include "TextureManager.h"
#include <cmath>
#include <algorithm>

constexpr Vector2 kNumberUVScale = { 0.2f,0.5f };

Vector2 ScoreUI::CalcShakeOffset(float t, float timeSec, int seed) const
{
	// t(0-1) が上がるほど、揺れ幅と速度を上げる
	float amp = 0.0f + 30.0f * (t * t);      // px
	float freq = 25.0f + 35.0f * t;          // Hz（ガタガタ震える感じ）

	float phase = seed * 1.37f;
	const float TAU = 6.2831853f;

	float x = amp * sinf((timeSec * freq * TAU) + phase);
	float y = amp * sinf((timeSec * (freq * 1.13f) * TAU) + phase);
	y *= 0.6f;

	return { x, y };
}

float ScoreUI::CalcDeltaTime()
{
	using clock = std::chrono::steady_clock;
	const auto now = clock::now();

	if (!hasPrevTick_)
	{
		hasPrevTick_ = true;
		prevTick_ = now;
		return 1.0f / 60.0f; // 初回は仮値
	}

	std::chrono::duration<float> diff = now - prevTick_;
	prevTick_ = now;

	// 変なスパイクを抑える（タブ復帰など）
	float dt = std::clamp(diff.count(), 0.0f, 0.05f);
	return dt;
}

void ScoreUI::Awake()
{
	constexpr Vector2 kNumberPos[ToIndex(ScoreDigits::Count)] = { {515.f,22.f},{575.f,22.f},{635.f,22.f},{695.f,22.f} };
	constexpr Vector2 kNumberSize = { 105.f,105.f };
	for (size_t digits = 0; digits < ToIndex(ScoreDigits::Count); ++digits)
	{
		GameObject& go = gameObject->CreateChild();
		ui_[digits] = &(go.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Score_Number, kNumberPos[digits], kNumberSize));
		ui_[digits]->SetUVScale(kNumberUVScale);
		basePos_[digits] = kNumberPos[digits];
	}

	constexpr Vector2 kAtoKgPos = { 463.f,68.f };
	constexpr Vector2 kAtoKgSize = { 410.f,55.f };

	GameObject& atoKg = gameObject->CreateChild();
	uiAtoKg_ = &(atoKg.AddComponent<UiSpriteRenderer>(TextureTag::Game_UI_Score_Text, kAtoKgPos, kAtoKgSize));
	baseAtoKgPos_ = kAtoKgPos;
}

void ScoreUI::Start()
{
	GameObject* scoreMgr = gameObject->world->FindWithTag(Tag::ScoreManager);
	score_ = scoreMgr->GetComponent<ScoreManager>();
}

void ScoreUI::Update()
{
	const float dt = CalcDeltaTime();
	shakeTimeSec_ += dt;

	// 揺れは時間経過で減衰（約0.25秒で収束）
	shakeStrength_ = (std::max)(0.0f, shakeStrength_ - dt * 2.f);

	if (!score_) return;

	const bool dirty = score_->GetDirty();
	const int sc = score_->GetScore();
	const bool changed = (sc != prevScore_);

	// スコアが更新されたら揺れを発生＆数字UVを更新
	if (dirty || changed)
	{
		if (changed)
		{
			TriggerShake(1.0f);
			prevScore_ = sc;
		}

		int digitsValues[4] = {
			(sc / 1000) % 10,
			(sc / 100) % 10,
			(sc / 10) % 10,
			(sc % 10)
		};

		const float cellW = 0.2f; // 1.0 / 5
		const float cellH = 0.5f; // 1.0 / 2

		for (int i = 0; i < 4; ++i)
		{
			int num = digitsValues[i];
			float u = (num % 5) * cellW;
			float v = (num / 5) * cellH;

			ui_[i]->SetUVPos({ u, v });
			ui_[i]->SetUVScale(kNumberUVScale);
		}
	}

	// 毎フレーム：揺れを座標に反映
	// UiSpriteRenderer は offset をスクリーン左上からのピクセル座標として使うので、SetOffset で揺らす。
	for (int i = 0; i < 4; ++i)
	{
		auto off = CalcShakeOffset(shakeStrength_, shakeTimeSec_, i);
		// UiSpriteRenderer は "offset" をスクリーン左上からのピクセル座標として描画する
		// ので、SetOffset で揺れを反映する。
		ui_[i]->SetOffset(Vector2(basePos_[i].x + off.x, basePos_[i].y + off.y));
	}

	if (uiAtoKg_)
	{
		auto off = CalcShakeOffset(shakeStrength_, shakeTimeSec_, 100);
		uiAtoKg_->SetOffset(Vector2(baseAtoKgPos_.x + off.x, baseAtoKgPos_.y + off.y));
	}
}

void ScoreUI::OnDestroy()
{
	uiAtoKg_ = nullptr;
	for (size_t digits = 0; digits < ToIndex(ScoreDigits::Count); ++digits)
	{
		ui_[digits] = nullptr;
	}
	score_ = nullptr;
	prevScore_ = -1;

	shakeStrength_ = 0.0f;
	shakeTimeSec_ = 0.0f;
	hasPrevTick_ = false;
}

void ScoreUI::TriggerShake(float strength)
{
	shakeStrength_ = std::clamp(strength, 0.0f, 1.0f);
}

void ScoreUI::SetShakeStrength(float t)
{
	shakeStrength_ = std::clamp(t, 0.0f, 1.0f);
}

