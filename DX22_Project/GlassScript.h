#pragma once
#include "Component.h"
#include "DirectXMath.h"

enum class GlassGrade : int
{
	Grade_1 = 0,
	Grade_2 = 1,
	Grade_3 = 2,
	Count = 3
};
constexpr int ToInt(GlassGrade g) { return static_cast<int>(g); }

// グレードごとのゲージの上がり値
constexpr float kGaugeUpValue[ToInt(GlassGrade::Count)] = { 2.f, 5.f, 10.f };
// グレードごとのKg(スコア変動値)
constexpr int kWeightKg[ToInt(GlassGrade::Count)] = { 1, 3, 5 };

constexpr DirectX::XMFLOAT4 kTintColor[ToInt(GlassGrade::Count)] = { {0.7f, 0.8f, 1.0f, 0.6f}, {0.7f, 1.0f, 0.7f, 0.6f}, {1.0f, 0.7f, 0.7f, 0.6f} };

class GlassScript : public MonoBehaviour
{
private:
	bool isBreaking = false;
	float breakTimer = 0.0f;
	const float breakDuration = 1.2f;
public:
	GlassScript(GlassGrade g = GlassGrade::Grade_1) : grade(g) {}
	void Start() override;
	void Update() override;
	void OnDestroy() override;
	GlassGrade grade = GlassGrade::Grade_1;
	bool isBrake() { return isBreaking; }
public:
	void SpawnShards();
	void StartBrake();
};