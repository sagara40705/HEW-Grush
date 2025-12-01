#pragma once
#include <cstdint>
#include "Component.h"

enum class HummerGrade : std::uint8_t
{
	Grade_1,
	Grade_2,
	Grade_3,
	Count
};
constexpr int ToInt(HummerGrade g) { return static_cast<size_t>(g); }

class GlassGaugeManager : public MonoBehaviour
{
private:
	HummerGrade grade_ = HummerGrade ::Grade_1;
	float gaude_ = 0.0f;
public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:
	void OnGlassBrake();
};