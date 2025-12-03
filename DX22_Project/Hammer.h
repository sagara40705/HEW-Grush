#pragma once
#include "Component.h"
#include <cstdint>

enum class HammerGrade : std::uint8_t
{
	Grade_1 = 0,
	Grade_2 = 1,
	Grade_3 = 2,
	Count = 3
};
constexpr int ToIndex(HammerGrade g) noexcept {
	return static_cast<size_t>(g); 
}
constexpr HammerGrade NextGrade(HammerGrade g) noexcept {
	int next = ToIndex(g) + 1;
	if (next >= ToIndex(HammerGrade::Count)) return g; // Ç±ÇÍà»è„ÅAè„Ç™ÇÁÇ»Ç¢
	return static_cast<HammerGrade>(next);
}
constexpr HammerGrade PrevGrade(HammerGrade g) noexcept{
	int prev = ToIndex(g) - 1;
	if (prev < 0) return g; // Ç±ÇÍà»è„ÅAâ∫Ç™ÇÁÇ»Ç¢
	return static_cast<HammerGrade>(prev);
}

class Hammer : public MonoBehaviour, public TriggerListener
{
private:
	HammerGrade grade_ = HammerGrade::Grade_1;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
	void OnTriggerStay(const TriggerInfo& info) override;
public:
	HammerGrade GetGrade() { return grade_; }
	void GradeUp();
	void GradeDown();
	void SetScale();
};