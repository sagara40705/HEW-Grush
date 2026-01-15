#pragma once
#include <cstdint>
#include "Component.h"
#include "ResultGaugeUI.h"
enum class Type : std::uint8_t
{
	S,
	A,
	B,
	C,
	D,
	Count
};

constexpr static size_t ToIndex(Type d) noexcept { return static_cast<size_t>(d); }

class ResultRankUI : public MonoBehaviour
{
public:

	ResultRankUI() = default;
	~ResultRankUI() = default;

	void Start();
	void Update(float);
	void Draw();
private:
	UiSpriteRenderer* uiMinute_[ToIndex(Type::Count)]{};
	UiSpriteRenderer* uiColon_ = nullptr;
	UiSpriteRenderer* uiSecond_[ToIndex(Type::Count)]{};
};
