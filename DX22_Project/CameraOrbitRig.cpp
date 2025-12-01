// === File: CameraOrbitPivot.cpp ===
#include "CameraOrbitRig.h"
#include "GameObject.h"
#include <cassert>

#ifdef _WIN32
#include <Windows.h>
static inline bool KeyHeld(int vk) { return (GetAsyncKeyState(vk) & 0x8000) != 0; }
#else
static inline bool KeyHeld(int) { return false; }
#endif

void CameraOrbitRig::Start()
{
	self_ = gameObject->TryGetComponent<Transform>();
	assert(self_ && "CameraOrbitPivot requires Transform");
	assert(player_ && "CameraOrbitPivot requires Player Transform");
	// 初期位置・回転を反映
	const auto p = player_->GetPosition();
	self_->SetPosition({ p.x, p.y + cfg_.height, p.z });
	self_->SetRotation({ 0.0f, 0.0f, cfg_.angleDeg }); // {roll,pitch,yaw}[deg]
}

void CameraOrbitRig::OnDestroy()
{
	self_ = nullptr;
	player_ = nullptr;
}

void CameraOrbitRig::Update()
{
	// Δtが取れるなら差し替え
	constexpr float DT = 1.f / 60.f;
	if (!self_ || !player_) return;

	// プレイヤー位置に高さオフセットだけ与えて固定
	const auto p = player_->GetPosition();
	self_->SetPosition({ p.x, p.y + cfg_.height, p.z });

	// 左右入力でYawのみ回転（プレイヤーは回さない）
	ApplyInput_(DT);
	self_->SetRotation({ 0.0f, cfg_.angleDeg, 0.0f});

}

void CameraOrbitRig::ApplyInput_(float dt)
{
	float d = 0.f;
#ifdef WIN32
	if (KeyHeld(VK_RIGHT)) d += cfg_.yawSpeedDeg * dt; // 時計回り
	if (KeyHeld(VK_LEFT)) d -= cfg_.yawSpeedDeg * dt; // 反時計回り
#endif
	if (d != 0.f) AddAngleDegrees(d);
}