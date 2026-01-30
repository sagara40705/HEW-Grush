#pragma once
#include "Component.h"
#include <cstdint>

namespace 
{
	enum class Point : std::uint8_t
	{
		AtoB,
		BtoA,
		CtoD,
		DtoC,
		Count
	};
	constexpr size_t ToIndex(Point p) noexcept { return static_cast<size_t>(p); }
}

/*
* ワープギミック
*/
class WarpPoint : public Component, public TriggerListener
{
private:
	Point point_ = Point::AtoB;
public:
	WarpPoint(Point point) : point_(point) {}
	void Start() override;
	void OnDestroy() override;
	void OnTriggerEnter(const TriggerInfo& info) override;
};