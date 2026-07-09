// === File: CameraFollow.h ===
#pragma once
#include "Component.h"
#include "Vector3.h"
#include <cstdint>
#include <cmath>

class Transfrom; class PlayerAction;

// 仕様：
// 1) 原点 = プレイヤー(ターゲット)位置 + (0, height, 0)
// 2) 原点を中心に半径=distance の水平円上を回る（右=時計回り、左=反時計回り）
// 3) カメラの向きは常にプレイヤー位置（原点ではなくプレイヤー本体）を向く
class CameraFollow final : public MonoBehaviour
{
public:
	struct Settings
	{
		float distance = 10.0f; // 半径
		float height = 3.0f; // 原点の上げ量（ターゲットYに加算）
		float angleDeg = 0.0f; // 現在角度[deg]（0度は +Z 方向）
		float yawSpeedDeg = 120.0f; // 角速度[deg/s]
	};

public:
	explicit CameraFollow(Transform* target, const Settings& s = Settings{})
		: target_(target), cfg_(s) {}

	void Start() override;
	void Update() override;
	void OnDestroy() override;

	// 外部から角度を加算（マウスなど）
	void AddAngleDegrees(float d) noexcept { cfg_.angleDeg += d; WrapAngle_(); }
	// 距離変更（ズーム）
	void SetDistance(float d) noexcept { if (d > 0.f) cfg_.distance = d; }


private:
	Transform* self_ = nullptr;
	Transform* target_ = nullptr;
	PlayerAction* pa_ = nullptr;
	Settings cfg_{};

private:
	void ApplyKeyboardInput_(); // 左右のみ
	void RecomputeTransform_(); // 位置計算＋LookAt（Transform::SetRotation(Vector3[deg])）
	void WrapAngle_() noexcept
	{
		while (cfg_.angleDeg >= 180.f) cfg_.angleDeg -= 360.f;
		while (cfg_.angleDeg < -180.f) cfg_.angleDeg += 360.f;
	}
};