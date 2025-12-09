#pragma once
#include "Component.h"
#include "Vector3.h"
#include <cstdint>

enum class HammerGrade : std::uint8_t
{
	Grade_1 = 0,
	Grade_2 = 1,
	Grade_3 = 2,
	Count = 3
};
constexpr size_t ToIndex(HammerGrade g) noexcept {
	return static_cast<size_t>(g); 
}
constexpr HammerGrade NextGrade(HammerGrade g) noexcept {
	size_t next = ToIndex(g) + 1;
	if (next >= ToIndex(HammerGrade::Count)) return g; // Ç±ÇÍà»è„ÅAè„Ç™ÇÁÇ»Ç¢
	return static_cast<HammerGrade>(next);
}
constexpr HammerGrade PrevGrade(HammerGrade g) noexcept{
	size_t prev = ToIndex(g) - 1;
	if (prev < 0) return g; // Ç±ÇÍà»è„ÅAâ∫Ç™ÇÁÇ»Ç¢
	return static_cast<HammerGrade>(prev);
}

class BoxCollider; class CameraShake;
class GlassObject; enum class GlassGrade : std::uint8_t;
class GlassGaugeManager; class ScoreManager;

class Hammer : public MonoBehaviour, public TriggerListener
{
public:
	static constexpr Vector3 BasePos = {0.f, 0.f, -1.5f};
	static constexpr Vector3 BaseRota = { 0.f, 0.f, 0.f };
private:
	HammerGrade grade_ = HammerGrade::Grade_1;
private:
	Transform* tf_ = nullptr;
	BoxCollider* bc_ = nullptr;
	CameraShake* shake_ = nullptr;
	GlassGaugeManager* gauge_ = nullptr;
	ScoreManager* score_ = nullptr;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
	void OnTriggerEnter(const TriggerInfo& info) override;
public:
	HammerGrade GetGrade() { return grade_; }
	void GradeUp();
	void GradeDown();
	void SetScale();
	void UpdateNormalAttack(float dt);
	void ResetPos();
private:
	void Attack(GameObject* go);
	bool CanBrake(GlassGrade glass);
private:
#ifdef _DEBUG
	void DebugCommand();
#endif // DEBUG	
};