#pragma once
#include "Component.h"
#include <cstdint>

class PlayerState;

enum class ActionDigits : std::uint8_t
{
	SpinMono,
	SpinFlash,
	Spin,
	SpinFront,
	DashMono,
	DashFlash,
	Dash,
	DashFront,
	Count
};

constexpr size_t ToIndex(ActionDigits d) noexcept { return static_cast<size_t>(d); }

class ActionUI : public MonoBehaviour
{
private: //éQè∆
	PlayerState* state_ = nullptr;
	UiSpriteRenderer* spinMoji_ = nullptr;
	UiSpriteRenderer* dashMoji_ = nullptr;
	UiSpriteRenderer* ActionUi_[ToIndex(ActionDigits::Count)]{};

	float glowDashTimer = 0.f;
	float glowSpinTimer = 0.f;
	bool wasSpinReady = false;
	bool wasDashReady = false;

public:
	void Awake() override;
	void Start() override;
	void Update() override;
	void OnDestroy() override;
public:


};

