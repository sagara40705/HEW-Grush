#pragma once
#include "Component.h"
#include <cstdint>
#include <chrono>
#include "Vector2.h"

enum class ScoreDigits : std::uint8_t
{
	One,
	Ten,
	Hundred,
	Thousand,
	Count
};
constexpr size_t ToIndex(ScoreDigits d) noexcept { return static_cast<size_t>(d); }


class ScoreManager;
class UiSpriteRenderer;

class ScoreUI : public MonoBehaviour
{
private:
	ScoreManager* score_ = nullptr;
	int prevScore_ = -1;
	UiSpriteRenderer* ui_[ToIndex(ScoreDigits::Count)]{};
	UiSpriteRenderer* uiAtoKg_ = nullptr;
private:
	// --- Shake (score) ---
	Vector2 basePos_[ToIndex(ScoreDigits::Count)];
	Vector2 baseAtoKgPos_;
	// Shake parameters
	float shakeStrength_ = 0.0f; // 0-1
	float shakeTimeSec_ = 0.0f;
	bool hasPrevTick_ = false;

	std::chrono::steady_clock::time_point prevTick_{};
	Vector2 CalcShakeOffset(float t, float timeSec, int seed) const;
	float CalcDeltaTime();

public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	// 揺れを手動で発生させたいときに呼ぶ（例: スコア加算演出など）
	void TriggerShake(float strength = 1.0f);
	// 揺れ強度を外部から直接設定したい場合（0-1）
	void SetShakeStrength(float t);
};