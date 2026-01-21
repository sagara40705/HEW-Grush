#pragma once
#include "Component.h"
#include <cstdint>

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

class ScoreUI : public MonoBehaviour
{
private:
	ScoreManager* score_ = nullptr;
	UiSpriteRenderer* ui_[ToIndex(ScoreDigits::Count)]{};
	UiSpriteRenderer* uiAtoKg_ = nullptr;
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
};