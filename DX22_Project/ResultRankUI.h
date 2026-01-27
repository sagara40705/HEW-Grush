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
struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	Vector4() : x(0), y(0), z(0), w(0) {}
	Vector4(float _x, float _y, float _z, float _w)
		: x(_x), y(_y), z(_z), w(_w) {}
};

constexpr static size_t ToIndex(Type d) noexcept { return static_cast<size_t>(d); }

class ResultRankUI : public MonoBehaviour
{
public:

	ResultRankUI() = default;
	~ResultRankUI() = default;

	void  Awake() override;
	void Update(float deltaTime);
	void Draw();
private:
	float glowTime_ = 0.f;
	UiSpriteRenderer* uiMinute_[ToIndex(Type::Count)]{};
	UiSpriteRenderer* uiColon_ = nullptr;
	UiSpriteRenderer* uiSecond_[ToIndex(Type::Count)]{};
	/*UiSpriteRenderer* rankBase_ = nullptr;
	UiSpriteRenderer* rankGlow_ = nullptr;*/
};
