#pragma once
#include "Component.h"

class PlayerState; class Hammer;

class PlayerAction : public MonoBehaviour
{
private:
	Transform* tf_ = nullptr;
	PlayerState* state_ = nullptr;
	Hammer* hammer_ = nullptr;
public:
	void Start() override;
	void Update() override;
	void OnDestroy() override;
private:
	void InputWantAction();
	void CheckCanAction();
	void UpdateAction();
	void UpdateActionCT();
private:
	void SetHammerActive(bool v);
	void UpdateNormalAttack();
	void UpdateSpinAttack();
	void UpdateDashAttack();
public:
	void StopNormalAttack();
};