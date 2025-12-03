#pragma once
#include "Component.h"
#include <cstdint>

enum class HummerGrade : std::uint8_t
{
	Grade_1 = 0,
	Grade_2 = 1,
	Grade_3 = 2,
	Count = 3
};
constexpr int ToIndex(HummerGrade g) noexcept {
	return static_cast<size_t>(g); 
}
constexpr HummerGrade NextGrade(HummerGrade g) noexcept {
	int next = ToIndex(g) + 1;
	if (next >= ToIndex(HummerGrade::Count)) return g; // Ç±ÇÍà»è„ÅAè„Ç™ÇÁÇ»Ç¢
	return static_cast<HummerGrade>(next);
}
constexpr HummerGrade PrevGrade(HummerGrade g) noexcept{
	int prev = ToIndex(g) - 1;
	if (prev < 0) return g; // Ç±ÇÍà»è„ÅAâ∫Ç™ÇÁÇ»Ç¢
	return static_cast<HummerGrade>(prev);
}

class Hammer : public MonoBehaviour, public TriggerListener
{
private:
	HummerGrade grade_ = HummerGrade::Grade_1;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
	void OnTriggerStay(const TriggerInfo& info) override;
public:
	HummerGrade GetGrade() { return grade_; }
	void GradeUp();
	void GradeDown();
	void SetScale();
};