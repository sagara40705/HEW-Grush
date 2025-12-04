
#pragma once
#include "Component.h"
#include "Transform.h"
#include "Vector3.h"
#include <cstdint>
#include <cmath>

class CameraOrbitRig final : public MonoBehaviour
{
public:
	struct Settings {
		float height = 3.0f; // ピボットを持ち上げる量
		float angleDeg = 0.0f; // 初期Yaw[deg]
		float yawSpeedDeg = 120.0f; // 角速度[deg/s]
	};

public:
	explicit CameraOrbitRig(Transform* player, const Settings& s = Settings{})
		: player_(player), cfg_(s) {}

	void Start() override;
	void OnDestroy() override;
	void Update() override;

	// 外部（マウス/パッド）から角度加算
	void AddAngleDegrees(float d) noexcept { cfg_.angleDeg += d; WrapAngle_(); }


private:
	Transform* self_ = nullptr; // ピボット自身（空オブジェクト）
	Transform* player_ = nullptr; // プレイヤー
	Settings cfg_{};

private:
	void ApplyInput_(float dt); // 左右のみ
	void WrapAngle_() noexcept {
		while (cfg_.angleDeg >= 180.f) cfg_.angleDeg -= 360.f;
		while (cfg_.angleDeg < -180.f) cfg_.angleDeg += 360.f;
	}
};