#pragma once
#include <cstdint>
#include "Component.h"
#include "Gauge.h"
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

class RankUI: public MonoBehaviour
{
public:
	
	RankUI() = default;
	~RankUI() = default;

	void Start();
	void Update(float);
	void Draw();
private:
	UiSpriteRenderer* uiMinute_[ToIndex(Type::Count)]{};
	UiSpriteRenderer* uiColon_ = nullptr;
	UiSpriteRenderer* uiSecond_[ToIndex(Type::Count)]{};
};

